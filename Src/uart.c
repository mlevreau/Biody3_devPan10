/*
 * uart.c
 *
 *  Created on: 01 Sept 2019
 *      Author: jyl
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "main.h"
#include "tim.h"
#include "hci.h"
#include "status.h"
#include "communication.h"
#include "uart.h"
#include "const.h"
#include "biody_message_manager.h"
#include "ble_message_manager.h"
#include "global_buffer.h"
#include "spp_over_ble.h"


struct uart_ctx *rs_ctx;
struct uart_ctx *ble_ctx;

#define CONSOLE_CMD_LENGTH   256
#define MAX_SERVICE_LOOP 256
#define NB_DATA 7

uint8_t  spl_requested = 0x0;
uint32_t ByteReceived = 0;

static void console_process_string(void* context);
static void uart_process_char(void* context);

/*
 * Modifications par Marilys L
 */

uint8_t *message;
uint8_t messageLength;

void init_data(uint8_t tableau[]){
	tableau[0] = 55;
	tableau[1] = 50;
	tableau[2] = 60;
	tableau[3] = 56;
	tableau[4] = 65;
	tableau[5] = 6;
	tableau[6] = 60;
}

void show_data(uint8_t tableau[]){
	int i;
	for( i=0; i<20; i++){
		 PRINT(rs_ctx,"%d,",tableau[i]);
	}
	 PRINT(rs_ctx,"\n");
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
   ByteReceived++;
	struct uart_ctx *ctx;
	if (huart==&USB_uart) {
		ctx = rs_ctx;
	} else if (huart==&BLE_uart) {
		ctx = ble_ctx;
	} else ctx = NULL;

	if (ctx) {
	  if (ctx->RxFull==0) {
		ctx->rx_buffer[ctx->RxHead] = ctx->RxChar;        //get received character
		if(ctx==ble_ctx) PRINT(rs_ctx,">0x%x\n", ctx->RxChar);
		HAL_UART_Receive_IT(huart,&ctx->RxChar,1);      //prepare for next reception

		ctx->RxHead++;
		if (ctx->RxHead >= ctx->RxSize) ctx->RxHead = 0;
		if (ctx->RxHead == ctx->RxTail) ctx->RxFull = 0x1;
	  }
	}
}

uint8_t uart_transmit_buffer(void* context){
  struct uart_ctx *ctx = context;
  if (ctx) {
	int len = uart_tx_pending(ctx);
	len = min(len,TRANSMIT_SIZE);
	if (len>0) {
	  if (HAL_UART_Transmit_IT(ctx->huart,&ctx->tx_buffer[ctx->TxTail],len)==HAL_OK) {
		ctx->TxTail+=len;
		ctx->TxFull=0x0;
		if (ctx->TxTail >= ctx->TxSize) ctx->TxTail -= ctx->TxSize;
	  } else return 3;
	} else return 2;
    return 1;
  } else return 0;
}

struct uart_ctx* uart_init(UART_HandleTypeDef *huart, uint32_t rx_size, uint32_t tx_size) {
	struct uart_ctx *context;
	context = malloc(sizeof(struct uart_ctx));
	context->huart   = huart;
	context->RxSize  = rx_size;
	context->TxSize  = tx_size;
	context->RxHead  = 0;
	context->TxHead  = 0;
	context->RxTail  = 0;
	context->TxTail  = 0;
	context->RxFull  = 0x0;
	context->TxFull  = 0x0;
	context->rx_buffer =  malloc(rx_size);
	context->tx_buffer =  malloc(tx_size);
	context->console_ptr = 0;
	context->console_str = malloc(CONSOLE_CMD_LENGTH);

    NbCarReception1 = 0;

	HAL_UART_Receive_IT(huart,&context->RxChar,1);

	return (context);
}

uint8_t uart_tx_empty(void* context) {
  struct uart_ctx *ctx = context;
  if (ctx) {
    return ((ctx->TxHead == ctx->TxTail) && (ctx->TxFull==0x0));
  } else return 0;
}

int uart_tx_pending(void* context) {
  struct uart_ctx *ctx = context;
  int len=0;
  if (ctx) {
    if (!uart_tx_empty(context)) {
	  if (ctx->TxHead>ctx->TxTail) {
	    len = ctx->TxHead-ctx->TxTail;
	  } else {
	    len = ctx->TxSize-ctx->TxTail; // don't roll over the end of buffer when transmitting
	  }
    }
  }
  return len;
}

int uart_tx_free4transmit(void* context) {
  int len=0;
  struct uart_ctx *ctx = context;
  if (ctx) {
	len = ctx->TxSize-uart_tx_pending(context);
  }
  return len;
}

uint8_t uart_rx_empty(void* context) {
	struct uart_ctx *ctx = context;
	if (ctx) {
	  return ((ctx->RxHead == ctx->RxTail) && (ctx->RxFull==0x0));
	} else return 0;
}

int uart_rx_received(void* context) {
	struct uart_ctx *ctx = context;
	int cnt = -1;
	if (ctx) {
		if (uart_rx_empty(context)==0x1) {
			cnt = 0;
		} else if (ctx->RxFull==0x1) {
			cnt = ctx -> RxSize;
		} else if (ctx->RxHead >= ctx->RxTail) {
			cnt = ctx->RxHead - ctx->RxTail;
		} else {
			cnt = (ctx -> RxSize+1) + ctx->RxHead - ctx->RxTail;
		}
	};
	return cnt;
}

uint8_t uart_tx_full(void* context) {
	struct uart_ctx *ctx = context;
	if (ctx) {
		return (ctx->TxFull);
	} else return 0;
}

uint8_t uart_get_char(void* context) {
	struct uart_ctx *ctx = context;
	if (ctx) {
		uint8_t rxChar = ctx->rx_buffer[ctx->RxTail];
		ctx->RxTail++;
		if (ctx->RxTail >= ctx->RxSize) ctx->RxTail=0;
		if (ctx->RxFull==0x1) ctx->RxFull=0x0;
		return rxChar;
	} else return 0;
}

uint8_t uart_put_char(void* context,uint8_t a_char) {
	struct uart_ctx *ctx = context;
	if (ctx) {
		if (uart_tx_full(context)) return (-1);
		ctx->tx_buffer[ctx->TxHead] = a_char;
		ctx->TxHead++;
		if (ctx->TxHead >= ctx->TxSize) ctx->TxHead = 0;
		if (ctx->TxHead == ctx->TxTail) ctx->TxFull = 0x1;
		return(1);
	} else return -2;
}

void uart_put_string(void* context,char *a_string) {
  int loop = 0;
  while (loop < strlen(a_string)) {
	uart_put_char(context,a_string[loop++]);
  }
}

uint8_t check_sum(char *bfr,int length){
  char cs = 0;
  int q;
  for (q=0;q<length;q++){ cs ^= *bfr++;}
  return (cs);
}

void uart_put_buffer(void* context,char *bfr,int length) {
  int loop = 0;
  while (loop++ < length) {
	  //while (uart_tx_full());
	  uart_put_char(context,*(bfr++));
  }
}

uint8_t uart_get_buffer(void* context,char *bfr,int length, char checksum,int duration10ms){
  int pp10mm = timer.pp10ms+duration10ms;
  while ((uart_rx_received(context)<length) && (timer.pp10ms<pp10mm)) {
	//wav_service();
  }
  if (timer.pp10ms<pp10mm) {
    int q;
    char cs = 0;
    for (q=0;q<length;q++){
      uint8_t c = uart_get_char(context);
      cs ^= c;
      *bfr++ = c;
    }
    if (cs!=checksum) PRINT(rs_ctx,"wrong checksum (0x%x-0x%x)\n",cs,checksum);
    return (cs==checksum);
  }
  PRINT(rs_ctx,"not enough byte received (%d)\n",uart_rx_received(context));
  return 0;
}

//specific for BLE
uint8_t UART_retrieveBytes(uint8_t *bfr,int length, int duration1ms){
  uint8_t ret = STATUS_ERROR;
  int ppmm = timer.ppms+duration1ms*UART_MS_TIME_RATIO;
  uart_service_ms();
  while ((uart_rx_received(ble_ctx)<length) && (timer.ppms<ppmm)) {
	uart_service_ms();
  }

  if (timer.ppms<ppmm) {
	  int q;
	   for (q=0;q<length;q++){
	     uint8_t c = uart_get_char(ble_ctx);
	     *bfr++ = c;
	   }
    ret = STATUS_SUCCESS;
  } else {
	  PRINT(rs_ctx,"exp:%d rec:%d\n",uart_rx_received(ble_ctx),length);
	ret = STATUS_TIMEOUT;
  }
  PRINT(rs_ctx,"(UART_retrieveBytes)ret = %s\n",(ret==0)?"SUCCESS":"ERROR");
   return ret;
}

uint8_t UART_sendBytes(uint8_t *bfr,int length) {
  int loop = 0;
  while (loop++ < length) {
	  while (uart_tx_full(ble_ctx));
     uart_put_char(ble_ctx,*(bfr++));
     //PRINT(rs_ctx, "<0x%x\n",*(bfr++));
  }
  while (uart_tx_pending(ble_ctx)) uart_service_ms();
  return STATUS_SUCCESS;
}
//end specific to BLE

void show_version(){
  PRINT(rs_ctx,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  PRINT(rs_ctx," BIODY XPERT-3 SW:%s\n",SOFT_VERSION);
  PRINT(rs_ctx,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

    // Modifi� par Marilys L
static void console_process_string(void* context) {
  struct uart_ctx *ctx = context;
  int res;
  uint8_t tab[NB_DATA];

  if (ctx==rs_ctx) {
    if (strncmp(ctx->console_str,"help",4)==0) {
      PRINT(ctx,"no help available at this time...\n");
    } else if (strncmp(ctx->console_str,"ver",3)==0){
  	  show_version();
    } else if (strncmp(ctx->console_str,"ble",3)==0){
  	  int x;
      if (sscanf(ctx->console_str+3,"%d",&x)==1) {
  	    HAL_GPIO_WritePin(BLE_RST_GPIO_Port,BLE_RST_Pin,0);
        delay_ms(1000); //leave time for PAN1026 to wake up
  	    HAL_GPIO_WritePin(BLE_RST_GPIO_Port,BLE_RST_Pin,x);
        delay_ms(1000); //leave time for PAN1026 to wake up
        int ret = COM_initAll();
        PRINT(ctx,"COM_initAll: %s\n",(ret==0)?"SUCCESS":"ERROR");
      }
    }else if (strncmp(ctx->console_str,"transfer",3)==0){

    	TableMesureGrandeursZPHI1[M5k][MZ] = 675;
    	TableMesureGrandeursZPHI1[M20k][MZ] = 660;
    	TableMesureGrandeursZPHI1[M50k][MZ] = 608;
    	TableMesureGrandeursZPHI1[M100k][MZ] = 560;
    	TableMesureGrandeursZPHI1[M200k][MZ] = 0x023A;

    	TableMesureGrandeursZPHI1[M5k][MPHI] = 59;
    	TableMesureGrandeursZPHI1[M20k][MPHI] = 65;
    	TableMesureGrandeursZPHI1[M50k][MPHI] = 71;
    	TableMesureGrandeursZPHI1[M100k][MPHI] = 81;
    	TableMesureGrandeursZPHI1[M200k][MPHI] = 57;

    	while (uart_rx_received(ble_ctx)>0) uart_get_char(ble_ctx);
    	res = SPPOBLE_manageProfile();
    	PRINT(ctx,"SPPOBLE_manageProfile: %s\n",(res==0)?"SUCCESS":"ERROR");

    } else if (strncmp(ctx->console_str,"data",3)==0){
    	init_data(tab);
        PRINT(rs_ctx,"tab :");
    	show_data(tab);
    } else {
  	  PRINT(ctx,"Unknown command (%s)...\n",ctx->console_str);
    }
  } else if (ctx==ble_ctx) {
  }
}

static void uart_process_char(void* context) {
  struct uart_ctx *ctx = context;
  uint8_t c = uart_get_char(ctx);
  if ((c==0x0D) || (c==0x0A)) {
 	 ctx->console_str[ctx->console_ptr] = 0x00;
     if (ctx->console_ptr>1) {console_process_string(ctx);}
     ctx->console_ptr=0;
  } else {
    if (c>0x1F) {  // no control character in commands
    	ctx->console_str[ctx->console_ptr] = tolower(c);
      if (ctx->console_ptr<CONSOLE_CMD_LENGTH) (ctx->console_ptr)++;
    } else {
      switch (c) {
         case 0x08:
         {
           if (ctx->console_ptr>0) ctx->console_ptr--;
           break;
         }
       }
     }
  }
}

void uart_service(void) {
  int loop = MAX_SERVICE_LOOP;
  while (!uart_rx_empty(rs_ctx) && (loop-->0)) {
	  uart_process_char(rs_ctx);
  }
}

void uart_service_ms(void) {
  if (!uart_tx_empty(rs_ctx) && ((HAL_UART_GetState(rs_ctx->huart) & HAL_UART_STATE_BUSY_TX)==HAL_UART_STATE_READY)) {
	uart_transmit_buffer(rs_ctx);
  }
  if (!uart_tx_empty(ble_ctx) && ((HAL_UART_GetState(ble_ctx->huart) & HAL_UART_STATE_BUSY_TX)==HAL_UART_STATE_READY)) {
	uart_transmit_buffer(ble_ctx);
  }
}

