/*
 * BLE_MESSAGE_MANAGER.h
 *
 *  Created on: 25 Oct 2018
 *      Author: Adrien Mallet
 */

#ifndef __BLE_MESSAGE_MANAGER_H_
#define __BLE_MESSAGE_MANAGER_H_

#include "stdint.h"

#define MESSAGE_START_INDEX ((TCU_HEADER_LENGTH)+8)
#define CHARC_VALUE_HANDLE_START_INDEX ((TCU_HEADER_LENGTH)+6)
#define GET_SERVICE_ID (reception_buffer[3])
#define GET_OP_CODE (reception_buffer[4])
#define GET_PARAM_LENGTH(data) ((data[2+TCU_HEADER_LENGTH] & 0xFF) | (((data[3+TCU_HEADER_LENGTH]) & 0xFF) << 8))       // +3 for header offset 
#define GET_MSG_LENGTH(data) ((GET_PARAM_LENGTH(data))-4)
#define GET_DISCONNECTION_REASON(data) (data[TCU_HEADER_LENGTH+7])

#define BLE_MAX_PACKET_SIZE 20

uint8_t BLEMSM_messageReceived;
uint8_t BLEMSM_messageReceivedLength;

uint8_t BLEMSM_previousMessagePacketToSend ;
uint8_t BLEMSM_previousMessagePacketToSendLength;

// handles
uint8_t BLEMSM_connectionHandle[2];
uint8_t BLEMSM_characteristicValueHandle[2];
uint8_t BLEMSM_characteristicDescriptorHandle[2];
uint8_t BLEMSM_characteristicDescriptorValue[2];
uint8_t BLEMSM_serverRxMtuSize[2];

uint8_t BLEMSM_deviceId[6];

/**
 * 
 * @param message : pointer of message to manages
 * @return status
 */
int BLEMSM_handleBleMessage(void);

/**
 * @brief : manage all message for LE MANAGER service 
 * @return 
 */
int BLEMSM_manageLeServiceManager(void);

int BLEMSM_manageLeConnectionCompleteEvent(void);
int BLEMSM_manageLeConnectionUpdateEvent(void);


int BLEMSM_manageLeServiceGattServer(void);
int BLEMSM_manageLeGattServerWriteCharValueEvent(void);
int BLEMSM_manageLeGattSdbUpdateCharEleResponse(void);
int BLEMSM_manageLeGattServerWriteCharValAcceptResp(void);
int BLEMSM_manageLeGattServerWriteCharDespEvent(void);
int BLEMSM_manageLeGattServerReadCharDespEvent(void);
int BLEMSM_manageLeGattServerReadMultipleEvent(void);
int BLEMSM_manageLeGattServerExgMtuEvent(void);
int BLEMSM_manageLeGattServerReadCharValueEvent(void);

int BLEMSM_manageLeAccept(void);
int BLEMSM_manageLeDisconnectEvent(void);

/**
 * @brief used to send by indications values
 * @param message : a buffer of bytes 
 * @param messageLength : length of the message
 * @return status
 */
int BLEMSM_manageLeGattServerCharValIndicationReq(uint8_t *message, uint8_t messageLength);

/**
 * @brief set a ble type response header with a success status
 * @param dest, buffer to set the header
 */
void BLEMSM_setResponseHeaderWithStatusSuccess(uint8_t *dest);

#endif /* __BLE_MESSAGE_MANAGER_H_ */
