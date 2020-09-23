/*
 * tcu.h
 *
 *  Created on: 19 oct. 2018
 *      Author: adrien
 *  Adapted on: Jun 05 2020 by JYL
 *
 */

#ifndef __TCU_H
#define __TCU_H

#include "stdint.h"
#include "global_buffer.h"

#define TCU_HEADER_LENGTH 3
#define SERVICE_ID_SIZE 1
#define OP_CODE_SIZE 1
#define PARAM_LENGTH_SIZE 2

#define TCU_RESPONSE_SUCCESS 0x00
#define TCU_RESPONSE_DEVICE_INIT_FINISHED 0x02

#define TCU_STATUS_ERROR (reception_buffer[4+TCU_HEADER_LENGTH] != TCU_RESPONSE_SUCCESS)	// 3(header) + 4
#define TCU_RESP_STATUS_ERROR (reception_buffer[6+TCU_HEADER_LENGTH] != TCU_RESPONSE_SUCCESS)
#define TCU_RESPONSE_BT_INIT_ERROR (reception_buffer[7] != TCU_RESPONSE_DEVICE_INIT_FINISHED)

/**
* @brief receive a response with tcu protocol
* if length expected == 0 this parameter is not tested
*
* param[in] *buf: the destination buffer to store the response
* param[in] length_expected : total length expected for the response
*
* @return 0 : success
*/
int TCU_receiveResponse(uint8_t *buf, uint8_t length_expected);

/**
 * @brief prepare and send a tcu response with all parameters
 * @param destBuf : destination buffer to set the whole response
 * @param buf : buffer of data to send in the response
 * @param bufSize : size to send
 * @param serviceId : tcu info
 * @param opCode : tcu info
 * @return status
 */
int TCU_sendResponse(uint8_t *destBuf, uint8_t *buf, uint16_t bufSize, uint8_t serviceId, uint8_t opCode);

#endif /* __TCU_H */
