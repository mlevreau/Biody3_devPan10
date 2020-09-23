/*
 * SPP_OVER_BLE.h
 *
 *  Created on: 24 Oct 2018
 *      Author: Adrien Mallet
 */

#ifndef __SPP_OVER_BLE_H_
#define __SPP_OVER_BLE_H_

#include "stdint.h"
#include "communication.h"

#define SPPOBLE_BD_ADDR_MINIMIZE_LENGTH 4


/**
 * @brief : handle the different states during the SPPOBLE processes
 * @param newState : the new state to manage
 * @return status
 */
int SPPOBLE_handleState(enum communicationState newState);

/**
 * @brief : create the spp over ble profile
 * 
 * @return 0: SUCCESS
 */
int SPPOBLE_createProfile(void);

/**
 * @brief : manage the initialization of ble profile and updates on profile
 * @return STATUS
 */
int SPPOBLE_initProfile(void);

/**
 * @brief update the data of characteristics (do that before creation of profile)
 */
void SPPOBLE_updateProfileData(void);

/**
 * @brief : wait for a connection with a master, during timeout secs
 * @param timeout: time in sec for waiting a connection
 * @return status
 */
int SPPOBLE_waitForConnection(uint8_t timeout);

/**
 * @brief : wait for a first exchange (write, ), during timeout secs
 * @param timeout
 * @return status
 */
int SPPOBLE_waitForExchange(uint8_t timeout);

/**
 * @brief: manage all exchanges until end of exchanges afer a first exchange, during timeout seconds
 * @param timeout
 * @return status
 */
int SPPOBLE_manageExchanges(uint8_t timeout);

/**
 * @brief : handle messages received by ble
 * 
 * @return status 
 */
int SPPOBLE_handleBleMessage(void);

/**
 * @brief manage the message received by sppoble, decrypt and create response
 * @param message : buffer of the message
 * @param length : 
 * @return status
 */
int SPPOBLE_manageMessage(uint8_t *message, uint8_t messageLength);

/**
 * @brief : manage all exchanges for the profile
 * 
 * @return 0: SUCCESS
 */
int SPPOBLE_manageProfile(void);

/**
 * @brief manage the communication error code after a manageMessage call
 * @param error
 */
void SPPOBLE_manageCommunicationError(uint8_t error);

/**
 * @brief manage message to send if previous message was too long 
 * @param messageToSendLength
 * @return 
 */
int SPPOBLE_managePreviousMessagePacketToSend(uint8_t *messageRemaining, uint8_t messageRemainingLength);

uint16_t SPPOBLE_getManufacturerNameCharHandleValue();

uint16_t SPPOBLE_getModelNumberCharHandleValue();

uint16_t SPPOBLE_getSystemIdCharHandleValue();

uint16_t SPPOBLE_getHardwareRevisionCharHandleValue();

#endif /* __SPP_OVER_BLE_H_ */