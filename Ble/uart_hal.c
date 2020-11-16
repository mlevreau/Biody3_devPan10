
#include "uart_hal.h"
#include "uart.h"
#include "stdint.h"
#include "status.h"

#include "const.h"
#include "pconfig.h"
#include "global_buffer.h"

int UART_checkDataAvailable(uint16_t timeout){
  int ppm = timer.ppms+timeout*UART_MS_TIME_RATIO;
  while ((timer.ppms<ppm)) {
	  if(!uart_rx_empty(rs_ctx)) {
		  return STATUS_SUCCESS;
	  }
  }

  return STATUS_TIMEOUT;

}

int UART_isDataToSendWaiting(){
	int res;

	res = uart_tx_pending(rs_ctx);


	return res;
}


