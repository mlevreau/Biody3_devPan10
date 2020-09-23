/*
 * tcu.c
 *
 *  Created on: 19 oct. 2018
 *      Author: adrien
 */

#include "tcu.h"
#include "stdint.h"
#include "trace.h"
#include "status.h"
#include "uart.h"

#undef MODULE_NAME
#define MODULE_NAME "TCU"

int TCU_receiveResponse(uint8_t *buf, uint8_t length_expected){

	// first retrieve the 3rd bytes
	int res = UART_retrieveBytes(buf, 3, 1000);
	uint32_t msg_len;
	
	if (res) {
		trace_error(MODULE_NAME, "uart_application_receive_receive_bytes() failed\n");
		return res;
	}

	// retrieve the total length of the data
	msg_len = buf[0];
	msg_len |= (buf[1] << 8);
	msg_len |= (buf[2] << 16);

	res = UART_retrieveBytes(buf + 3, msg_len-3, 1000);
	if (res) {
		trace_error(MODULE_NAME, "uart_application_receive_receive_bytes() failed\n");
	    return res;
	}

	if(length_expected){
		if (length_expected != msg_len){
			return res;
		}
	}

	return STATUS_SUCCESS;
}


int TCU_sendResponse(uint8_t *destBuf, uint8_t *buf, uint16_t bufSize, uint8_t serviceId, uint8_t opCode){
    
    uint8_t i;
    uint8_t totalLength = TCU_HEADER_LENGTH + SERVICE_ID_SIZE + OP_CODE_SIZE + PARAM_LENGTH_SIZE + bufSize; 

    PRINT(rs_ctx,"buf = a assembling_buffer:\n");
        show_data(buf);

     // prepare the response with mandatory elements
    destBuf[0] = totalLength;
    destBuf[1] = 0;
    destBuf[2] = 0;
    destBuf[3] = serviceId;
    destBuf[4] = opCode;
    destBuf[5] = totalLength - (TCU_HEADER_LENGTH + SERVICE_ID_SIZE + OP_CODE_SIZE + PARAM_LENGTH_SIZE);   // PARAM length total - previous byte including 5/6
    destBuf[6] = 0;

    for(i=0; i<bufSize; i++){
        destBuf[7+i] = buf[i];
    }

    // send the buffer
    PRINT(rs_ctx,"destbuf:\n");
        show_data(destBuf);

    return UART_sendBytes(destBuf, totalLength);
}
