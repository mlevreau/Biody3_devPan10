
/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __GLOBAL_H
#define	__GLOBAL_H

#include <stdint.h>

#define BIODY_MAX_MESSAGE_PACKETS_SIZE 2
#define UART_RECEPTION_BUFFER_SIZE 80
#define UART_SENDING_BUFFER_SIZE 200
#define RESPONSE_ASSEMBLING_BUFFER_SIZE 35
#define BIODY_MESSAGE_RECEPTION_BUFFER_SIZE 20
#define TCU_RECEPTION_BUFFER_SIZE 80
#define BIODY_MESSAGE_SENDING_BUFFER_SIZE (BIODY_MESSAGE_RECEPTION_BUFFER_SIZE*BIODY_MAX_MESSAGE_PACKETS_SIZE) // is equal to the max packets number to send a biody message

// ************** init of the global buffer **********************************
uint8_t uartReceptionArray_buffer[UART_RECEPTION_BUFFER_SIZE];           // uart buffer reception
uint8_t tcuReceptionArray_buffer[TCU_RECEPTION_BUFFER_SIZE];             // tcu buffer to store a tcu frame when received
uint8_t uartSendingArray_buffer[UART_SENDING_BUFFER_SIZE];               // buffer for uart sending
uint8_t responseAssemblingArray_buffer[RESPONSE_ASSEMBLING_BUFFER_SIZE]; // buffer where to assemble the response (Biody response protocol)
uint8_t messageArray_buffer[BIODY_MESSAGE_RECEPTION_BUFFER_SIZE];                  // buffer to store a biody message to manage 
uint8_t messageRemainingToSendArray_buffer[BIODY_MESSAGE_SENDING_BUFFER_SIZE];   // buffer to store entire message to send

extern uint8_t *reception_buffer;
extern uint8_t *assembling_buffer;
extern uint8_t *sending_buffer;
extern uint8_t *messageReceived_buffer;
extern uint8_t *messageRemainingTosend_buffer;


#endif	/* __GLOBAL_H */
