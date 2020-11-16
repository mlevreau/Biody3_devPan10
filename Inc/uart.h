/*
 * uart.h
 *
 *  Created on: 01 sept 2019
 *      Author: jyl
 */

#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include <stdio.h>
#include "main.h"

#define RX_BUFFER_SIZE  1024
#define TX_BUFFER_SIZE 32768
#define TRANSMIT_SIZE    256
#define BLE_BUFFER_SIZE 1024

// multiplicateur par rapport au vrai nb de ms
// timer.ppmm = nb_ms* UART_MS_TIME_RATIO
#define UART_MS_TIME_RATIO 30

#define PRINT(ctx, ... )					\
do { char msg [300]; 					\
     sprintf( msg,  __VA_ARGS__ );		\
     uart_put_string(ctx,msg);     		\
 } while (0);


struct uart_ctx* uart_init(UART_HandleTypeDef *huart, uint32_t rx_size, uint32_t tx_size);
uint8_t uart_transmit_buffer(void* context);
int uart_tx_pending(void* context);
uint8_t uart_tx_empty(void* context);
int uart_tx_free4transmit(void* context);
uint8_t uart_rx_empty(void* context);
int uart_rx_received(void* context);
uint8_t uart_tx_full(void* context);
uint8_t uart_get_char(void* context);
uint8_t uart_put_char(void* context,uint8_t a_char);
void uart_put_string(void* context,char *a_string);
uint8_t check_sum(char *bfr,int length);
void uart_put_buffer(void* context,char *bfr,int length);
uint8_t uart_get_buffer(void* context,char *bfr,int length, char checksum,int duration10ms);
uint8_t UART_retrieveBytes(uint8_t *bfr,int length, int duration1ms);
uint8_t UART_sendBytes(uint8_t *bfr,int length);
void show_version();
void show_data(uint8_t tab[]);
void init_data(uint8_t tab[]);
unsigned char TransfertMesure(void);
void uart_service(void);
void uart_service_ms(void);

struct uart_ctx {

	UART_HandleTypeDef *huart;

	uint32_t RxSize;
	uint32_t TxSize;

	uint32_t TxHead;
	uint32_t TxTail;
	uint8_t  TxFull;
	uint8_t *tx_buffer;

	uint8_t  RxChar;
	uint32_t RxHead;
	uint32_t RxTail;
	uint8_t  RxFull;
	uint8_t *rx_buffer;

	char    *console_str;
	uint16_t console_ptr;

};

extern struct uart_ctx *rs_ctx;
extern struct uart_ctx *ble_ctx;
extern uint32_t ByteReceived;
extern uint8_t ble_transparent;

#endif /* UART_H_ */
