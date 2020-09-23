
#include "global_buffer.h"


// ***************  init the pointers of buffers ***********************
uint8_t *assembling_buffer = responseAssemblingArray_buffer;
uint8_t *reception_buffer = tcuReceptionArray_buffer;
uint8_t *sending_buffer = uartSendingArray_buffer;
uint8_t *messageReceived_buffer = messageArray_buffer;
uint8_t *messageRemainingTosend_buffer = messageRemainingToSendArray_buffer;
