/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __RANDOM_H
#define	__RANDOM_H

#include "stdint.h"

/**
* @brief return a random byte value
*
* @return uint8_t a random value
*/
uint8_t RAND_getRandomByte(void);


#endif	/* __RANDOM_H */
