
#include "uart_hal.h"
#include "uart.h"
#include "stdint.h"
#include "status.h"

#include "const.h"
#include "pconfig.h"
#include "global_buffer.h"

int UART_checkDataAvailable(uint16_t timeout){

	uart_tx_empty(rs_ctx);
	return 0;
}

int UART_isDataToSendWaiting(){
	int res;

	res = uart_tx_pending(rs_ctx);


	return STATUS_TRUE;
}


