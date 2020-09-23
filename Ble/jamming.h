/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __JAMMING_H
#define	__JAMMING_H

#include "stdint.h"

/**
* @brief jam a buffer by adding a random uint8_t value after sequence bytes.
* the buffer given for jamming is directly modified.
*
* @param[in] *buffer: buffer to jam
* @param[in] buffer_size: size of the buffer to jam
* @param[in] *dest_buffer: final buffer containing buffer jammed
* @param[in] sequence: sequence of n bytes before a noise (if sequence=2 => B B N B B N) B=byte ok, N=noise
*
* @return uint8_t sate : 0 success
*/
uint8_t JAM_jamBuffer(uint8_t* buffer, uint8_t buffer_size, uint8_t* dest_buffer, uint8_t sequence);

/**
* @brief unjam a buffer by removing the random uint8_t value after sequence bytes.
* the buffer given for unjamming is directly modified.
*
* @param[in] *buffer: buffer to unjam
* @param[in] buffer_size: size of the buffer to unjam
* @param[in] *dest_buffer: final buffer containing buffer unjammed
* @param[in] sequence: sequence of n bytes before a noise (if sequence=2 => B B N B B N) B=byte ok, N=noise
*
* @return uint8_t sate : 0 success
*/
uint8_t JAM_unjamBuffer(uint8_t* buffer, uint8_t buffer_size, uint8_t* dest_buffer, uint8_t sequence);


#endif	/* __JAMMING_H */
