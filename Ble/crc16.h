/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __CRC16_H
#define	__CRC16_H

#include "stdint.h"


//uint16_t CRC16_calculateCrc16(const unsigned char* data,  unsigned char length);

/**
* @brief calculation of a crc16 CITT X MODEM
*
* @return uint16_t, the crc16 of the whole data
*/
uint16_t CRC16_calculateCrc16Xmodem(uint8_t *bytes, uint8_t length);

#endif	/* __CRC16_H */
