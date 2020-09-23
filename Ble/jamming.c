

#include "stdint.h"
#include "jamming.h"
#include "random.h"
#include "status.h"


uint8_t JAM_jamBuffer(uint8_t* buffer, uint8_t buffer_size, uint8_t* dest_buffer, uint8_t sequence){
	
	if(buffer == dest_buffer){
		return STATUS_ERROR;	// error same buffer
	}

	uint8_t bytes_to_add = buffer_size/sequence;		// bytes to add to the buffer
	uint8_t final_size = buffer_size+bytes_to_add;			// total of bytes
	
	uint8_t step = sequence;
	uint8_t index = 0;
	uint8_t index_dest_buffer = 0;

	while(index_dest_buffer < final_size){
		if(step == 0){
			step = sequence+1;		// +1 because of the decrementation after
			// add a random byte !
			dest_buffer[index_dest_buffer] = RAND_getRandomByte();
		}else{
			dest_buffer[index_dest_buffer] = buffer[index];
			index++;
		}
		index_dest_buffer++;
		step--;
	}

	return STATUS_SUCCESS;
}

uint8_t JAM_unjamBuffer(uint8_t* buffer, uint8_t buffer_size, uint8_t* dest_buffer, uint8_t sequence){

	if(buffer == dest_buffer){
		return STATUS_ERROR;	// error same buffer
	}

	uint8_t bytes_to_remove = buffer_size/(sequence+1);			// bytes to remove from the buffer
	uint8_t final_size = buffer_size - bytes_to_remove;		// total of bytes

	uint8_t step = sequence;
	uint8_t index = 0;
	uint8_t index_dest_buffer = 0;

	while(index_dest_buffer < final_size){
		if(step == 0){
			step = sequence+1;		// +1 because of the decrementation after
		}else{
			dest_buffer[index_dest_buffer] = buffer[index];
			index_dest_buffer++;
		}
		index++;
		step--;
	}

	return STATUS_SUCCESS;
}
