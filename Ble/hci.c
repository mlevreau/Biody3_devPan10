/*
 * hci.c
 *
 *  Created on: Jun 05, 2020
 *      Author: JYL
 *      Adapted from original development by Adrien Mallet
 */

#include "hci.h"
#include "uart.h"
#include "stdint.h"
#include "trace.h"
#include "status.h"

#undef MODULE_NAME
#define MODULE_NAME "HCI"


int HCI_receiveResponse(uint8_t *buf, uint8_t length_expected){
	
	// first retrieve the 3rd bytes
	int res = UART_retrieveBytes(buf, 3, 300);   
	uint8_t command_length;

	if (res) {
		trace_error(MODULE_NAME,"uart_application_receive_receive_bytes1() failed1: %d\n",res);
		return STATUS_ERROR;
	}

	// retrieve the total length of the data
	command_length = buf[2];
	
	res = UART_retrieveBytes(buf + 3, command_length, 300);
	if (res) {
		trace_error(MODULE_NAME,"uart_application_receive_receive_bytes2() failed2: %d\n",res);
	    return STATUS_ERROR;
	}
	
	if(length_expected){

		if (length_expected != 3+command_length){
			return STATUS_ERROR;
		}
	}

	return STATUS_SUCCESS;
}
