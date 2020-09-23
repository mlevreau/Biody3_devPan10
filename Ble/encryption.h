/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __ENCRYPTION_H
#define	__ENCRYPTION_H

#include "stdint.h"


/**
* @brief encrypt a buffer with a key of 4 bytes
* the buffer given for encryption is directly modified.
*
* @param[in] *buffer: buffer to encrypt
* @param[in] buffer_size: size of the buffer to encrypt
* @param[in] key: crypt key
*
* @return uint8_t sate : 0 success
*/
//uint8_t ENCRYPT_encryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint32_t key);

/**
* @brief encrypt a buffer with a key of 4 bytes
* the buffer given for encryption is directly modified.
*
* @param[in] *buffer: buffer to encrypt
* @param[in] buffer_size: size of the buffer to encrypt
* @param[in] key: crypt key[] bytes array of 4 bytes
*
* @return uint8_t sate : 0 success
*/
uint8_t ENCRYPT_encryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint8_t *key);

/**
* @brief decrypt a buffer with a key of 4 bytes
* the buffer given for decryption is directly modified.
*
* @param[in] *buffer: buffer to decrypt
* @param[in] buffer_size: size of the buffer to decrypt
* @param[in] key: crypt key
*
* @return uint8_t state 0 success
*/
//uint8_t ENCRYPT_decryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint32_t key);

/**
* @brief decrypt a buffer with a key of 4 bytes
* the buffer given for decryption is directly modified.
*
* @param[in] *buffer: buffer to decrypt
* @param[in] buffer_size: size of the buffer to decrypt
* @param[in] key: crypt key [4]
*
* @return uint8_t state 0 success
*/
uint8_t ENCRYPT_decryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint8_t *key);


#endif	/* __ENCRYPTION_H */
