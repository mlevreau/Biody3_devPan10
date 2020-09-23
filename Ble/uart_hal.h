/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __UART_HAL_H
#define	__UART_HAL_H

#include "stdint.h"


/**
* @brief init the uart
*
* @return 0 : success
*/
int UART_init(void);

/**
 * @brief set the baud rate
 * @param baudRate
 */
void UART_setBaudRates(uint16_t baudRate);

/**
* @brief send bytes via uart with specific length
*
* param[in] *buf: buffer of data to send
* param[in] length: length of the buffer to send
*
* @return 0 : success
*/
//int UART_sendBytes(const uint8_t *buf, uint8_t length);

/**
* @brief check if data are available in uart
*
* param[in] timeout: in mseconds
*
* @return 0 : data available
*/
int UART_checkDataAvailable(uint16_t timeout);

/**
 * @brief check if data are wainting to be send by uart
 * @return STATUS_TRUE / STATUS_FALSE
 */
int UART_isDataToSendWaiting();

/**
* @brief retrieve bytes from buffer with a specific length and a timeout
*
* param[in] *buf: buffer where to add the bytes received
* param[in]	length: length of data to retrieve
* param[in] timeout: in mseconds
*
* @return 0 : data available
*/
//int UART_retrieveBytes(uint8_t *buf, uint32_t length, uint16_t timeout);


#endif	/* __UART_HAL_H */
