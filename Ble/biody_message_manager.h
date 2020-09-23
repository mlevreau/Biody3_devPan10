/*
 * BIODY_MESSAGE.h
 *
 *  Created on: 29 Oct 2018
 *      Author: Adrien Mallet
 */

#ifndef __BIODY_MESSAGE_MANAGER_H_
#define __BIODY_MESSAGE_MANAGER_H_

#include "stdint.h"

#define BIOMSGM_HEADER_SIZE 2
#define BIOMSGM_FOOTER_SIZE 2
#define BIOMSGM_HEADER_AND_FOOTER_SIZE 4
#define SIMPLE_REQUEST_CRC16_LOW_BYTE_INDEX 3
#define SIMPLE_REQUEST_CRC16_HIGH_BYTE_INDEX 2
#define PAYLOAD_SIZE_INDEX 1
#define PAYLOAD_START_INDEX 2

// ************* Macro for biody ***************
#define GET_CRC16_REQUEST(data, startIndex) (data[startIndex]<<8 | data[startIndex+1])
#define GET_CRC16_SIMPLE_REQUEST(data) ((data[SIMPLE_REQUEST_CRC16_LOW_BYTE_INDEX] | data[SIMPLE_REQUEST_CRC16_HIGH_BYTE_INDEX]<<8))
#define GET_PAYLOAD_SIZE_REQUEST(data) (data[PAYLOAD_SIZE_INDEX])
#define BIOMSGM_END_OF_EXCHANGES (BIOMSGM_biodyCurrentComState == BIOCOM_END_OF_EXCHANGES)
#define GET_CMD(data) (data[0])

// *************** Status for biody messages ***************
#define BIOSTATUS_CRC_ERROR 0xb0               // b for biody + value
#define BIOSTATUS_FRAME_ERROR 0xb1       
#define BIOSTATUS_PROCESS_ERROR 0xb2
#define BIOSTATUS_EXCHANGE_KEY_ERROR 0xb3
#define BIOSTATUS_TIMEOUT_REACHED_ERROR 0xb4

enum biodyCommands{
    BIOCMD_ASSOCIATION_REQ = 'a',           // jamm + crypt
    BIOCMD_EXCHANGE_KEY_REQ = 'e',          // nothing
    BIOCMD_MEASURES_REQ = 'p',              // nothing
    BIOCMD_TRANSFER_KEY_REQ = 'c',             // jamm + crypt
    BIOCMD_TRANSFER_KEY_VERIF_REQ = 't',    // j + c 
    BIOCMD_INFO_REQ = 'i',                  // nothing
    BIOCMD_DEV_INFO_REQ = 'k',              //nothing
    BIOCMD_MEASURES_ACKN = 'f',             // nothing
    BIOCMD_ASSOCIATION_RESP = 'b',          // jamm + crypt
    BIOCMD_CRYPT_KEY_RESP = 'd',            // j + c
    BIOCMD_TRANSFER_KEY_VERIF_RESP = 'u',   // j + c
    BIOCMD_MEASURES_RESP = 'r',             // j + c
    BIOCMD_INFO_RESP = 'j',                 // j + c
    BIOCMD_DEV_INFO_RESP = 'l',             // j+ c
    BIOCMD_STATUS_RESP = 's'                // nothing    
};

// state for biody protocol
enum BIOMSGM_biodyCommunicationState{
    BIOCOM_NONE,
    BIOCOM_ASSOCIATED,
    BIOCOM_TRANSFER_KEY_CREATED,
    BIOCOM_MEASURES_SEND,
    BIOCOM_MEASURES_RECEIVED,
    BIOCOM_END_OF_EXCHANGES
};

enum biodyStatusFrameCode{
    BIOSTATCD_SUCCESS = 0x00,
    BIOSTATCD_WRONG_FRAME = 0x01,
    BIOSTATCD_WRONG_CRC = 0x02,
    BIOSTATCD_PROCESS_FAILED = 0x03,
    BIOSTATCD_WRONG_EXCHANGE_KEY = 0x04,
    BIOSTATCD_TIMEOUT_REACHED = 0x05
};

extern enum BIOMSGM_biodyCommunicationState BIOMSGM_biodyCurrentComState;

/**
 * @brief manage a biody message
 * @param message : buffer with the entire message
 * @return status
 */
int BIOMSGM_manageMessage(uint8_t *message, uint8_t messageLength);

/**
 * @brief the whole association request, decrypt and create the response
 * @param message
 * @return status
 */
int BIOMSGM_manageAssociationRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief the whole information request, create the response
 * @param message
 * @return status
 */
int BIOMSGM_manageInformationRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief the whole developpers information request, create the response
 * @param message
 * @return status
 */
int BIOMSGM_manageBuildInformationRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief the whole measures request, create the response
 * @param message
 * @return status
 */
int BIOMSGM_manageMeasuresRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief the whole crypt key request, decrypt and create the response
 * @param message
 * @return status
 */
int BIOMSGM_manageTransferKeyRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief 
 * @param message
 * @param messageLength
 * @return 
 */
int BIOMSGM_manageTransferKeyVerifRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief the whole measures acknolegement request, change the BCOM state
 * @param message
 * @return status
 */
int BIOMSGM_manageMeasuresAcknowledgement(void);

/**
 * 
 * @param message
 * @param messageLength
 * @return 
 */
int BIOMSGM_manageExchangeKeyRequest(uint8_t *message, uint8_t messageLength);

/**
 * @brief send error frame, with an eror code
 * @param error, see enum biodyCommunicationError
 * @return status
 */
int BIOMSGM_sendStatusResponse(uint8_t error);


/**
 * @brief verify the crc of a message
 * @param message, biody message
 * @param messageLength total length of biody message
 * @return status
 */
int BIOMSGM_verifyCrc16(uint8_t *message, uint8_t messageLength);

/**
 * @brief verify if the message is a correct biody message (check cmd, length, crc)
 * @param message
 * @param messageLength
 * @return status (BIOSTATUS_* if error, STATUS_SUCCESS if ok)
 */
int BIOMSGM_isValidBiodyMessage(uint8_t *message, uint8_t messageLength);

/**
 * @brief send a biody message via indication
 * @param message, buffer of message to send
 * @param messageSize
 * @return status
 */
int BIOMSGM_sendMessage(uint8_t *message, uint8_t messageSize);

/**
 * @brief save a 4 bytes exchange key
 * @param key
 */
void BIOMSGM_saveExchangeKey(uint8_t *key);

/**
 * @brief load the previous exchange key 
 * @param destKey, the destination to load the previous key
 */
void BIOMSGM_loadPreviousExchangeKey(uint8_t *destKey);

/**
 * @brief create the transfer key (4bytes), combining na et nb, with a calculation for encryption
 * take the 3rd bits of low value for each element, combine those bits to create a number value between 0-3
 * for exemple na=5=b101 / nb=3=b011 so value to start index is 2=b010
 * @param na, first value to encrypt with
 * @param nb, second value to encrypt with
 * @param destKey, the result of the encryption
 */
void BIOMSGM_createTransferCryptKey(uint8_t *na, uint8_t *nb, uint8_t *destKey);

/**
 * @brief load the master key from eeprom
 * @param destKey
 */
void BIOMSGM_loadMasterKey(uint8_t *destKey);

/**
 * @brief save in EEPROM a new master key
 * @param key
 */
void BIOMSGM_saveMasterKey(uint8_t *key);

/**
 * @brief check the master key in eeprom and init it if factory value
 */
void BIOMSGM_manageMasterKey(void);


#endif /* __BIODY_MESSAGE_MANAGER_H_ */
