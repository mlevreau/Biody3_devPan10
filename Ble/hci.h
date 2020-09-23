/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __HCI_H
#define	__HCI_H

#include "stdint.h"

/**
* @brief receive a response with hci protocol
* if length expected == 0 this parameter is not tested
*
* param[in] *buf: the destination buffer to store the response
* param[in] length_expected : total length expected for the response
*
* @return 0 : success
*/
int HCI_receiveResponse(uint8_t *buf, uint8_t length_expected);


#endif	/* __HCI_H */
