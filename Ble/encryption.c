

#include "encryption.h"
#include "status.h" 
#include "stdint.h"


/*uint8_t ENCRYPT_encryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint32_t key){

	// change the uint 32 to an array
	uint8_t key_array[4];
	key_array[0] = key >> 24;
	key_array[1] = key >> 16;
	key_array[2] = key >> 8;
	key_array[3] = key;

	for(int i=0; i<buffer_size; i++){
		buffer[i] = buffer[i] ^ key_array[i%4];
	}

	return STATUS_SUCCESS;
}*/

uint8_t ENCRYPT_encryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint8_t *key){

	for(int i=0; i<buffer_size; i++){
		buffer[i] = buffer[i] ^ key[i%4];
	}

	return STATUS_SUCCESS;
}

/*uint8_t ENCRYPT_decryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint32_t key){

	return ENCRYPT_encryptBuffer(buffer, buffer_size, key);
}*/

uint8_t ENCRYPT_decryptBuffer(uint8_t *buffer, uint8_t buffer_size, uint8_t *key){

	return ENCRYPT_encryptBuffer(buffer, buffer_size, key);
}
