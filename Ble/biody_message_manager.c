
#include "biody_message_manager.h"
#include "stdint.h"
#include "crc16.h"
#include "encryption.h"
#include "jamming.h"
#include "random.h"
#include "status.h"
#include "global_buffer.h"
#include "const.h"
#include "pconfig.h"
#include "uart.h"
#include "version.h"
#include "global.h"

#include "ble_message_manager.h"
#include "tcu.h"
#include "EEP.h"

enum BIOMSGM_biodyCommunicationState BIOMSGM_biodyCurrentComState = BIOCOM_NONE;

// **** Crypt keys ******
uint8_t masterCryptKey[4];
uint8_t exchangeCryptKey[4] = {0x04, 0x03, 0x02, 0x01};
uint8_t transferCryptKey[4] = {0x01, 0x01, 0x01, 0x01};

// master key
const uint8_t DEFAULT_MASTER_CRYPT_KEY[4] = {0x3F, 0x28, 0x4C, 0xB7};

uint8_t randomN0[4];
uint8_t randomN1[4];
uint8_t challengeC0[4];

uint8_t jamSequence = 4;

int BIOMSGM_manageMessage(uint8_t *message, uint8_t messageLength){

    uint8_t res=0;
    enum BIOMSGM_biodyCommunicationState biodyNewStateTemp = BIOCOM_NONE;

    // check if correct biody message
    res = BIOMSGM_isValidBiodyMessage(message, messageLength);

    if(res != STATUS_SUCCESS){
    	PRINT(rs_ctx,"message non valide\n");
       return res;
    }
    
    // init the pointer for message storage
    //messageRemainingTosend_buffer = messageRemainingToSendArray_buffer;
    
    switch(GET_CMD(message)){
        case BIOCMD_ASSOCIATION_REQ:
        	PRINT(rs_ctx,"1");
            if(BIOMSGM_biodyCurrentComState >= BIOCOM_ASSOCIATED)
                return BIOSTATUS_PROCESS_ERROR;
            res = BIOMSGM_manageAssociationRequest(message, messageLength);
            break;
        case BIOCMD_EXCHANGE_KEY_REQ:
        	PRINT(rs_ctx,"2");
            res = BIOMSGM_manageExchangeKeyRequest(message, messageLength);
            biodyNewStateTemp = BIOCOM_ASSOCIATED;
            break;
        case BIOCMD_TRANSFER_KEY_REQ:
        	PRINT(rs_ctx,"3");
            res = BIOMSGM_manageTransferKeyRequest(message, messageLength);
            break;
        case BIOCMD_TRANSFER_KEY_VERIF_REQ:
        	PRINT(rs_ctx,"4");
            res = BIOMSGM_manageTransferKeyVerifRequest(message, messageLength);
            biodyNewStateTemp = BIOCOM_TRANSFER_KEY_CREATED;
            break;
        case BIOCMD_MEASURES_REQ:
        	PRINT(rs_ctx,"5");
            if(BIOMSGM_biodyCurrentComState < BIOCOM_TRANSFER_KEY_CREATED) // if no transfer key created, do not manage request yet !
                return BIOSTATUS_PROCESS_ERROR;
            res = BIOMSGM_manageMeasuresRequest(message, messageLength);
            biodyNewStateTemp = BIOCOM_MEASURES_SEND;
            break;
        case BIOCMD_MEASURES_ACKN:
        	PRINT(rs_ctx,"6");
            if(BIOMSGM_biodyCurrentComState < BIOCOM_MEASURES_SEND)
                return BIOSTATUS_PROCESS_ERROR;
            res = BIOMSGM_manageMeasuresAcknowledgement();
            biodyNewStateTemp = BIOCOM_END_OF_EXCHANGES;
            break;
        case BIOCMD_INFO_REQ:
        	PRINT(rs_ctx,"7");
            res = BIOMSGM_manageInformationRequest(message, messageLength);
            break;
        case BIOCMD_DEV_INFO_REQ:
        	PRINT(rs_ctx,"8");
            res = BIOMSGM_manageBuildInformationRequest(message, messageLength);
            break;
        default:
        	PRINT(rs_ctx,"9\n");
            return BIOSTATUS_FRAME_ERROR;
            break;
    }
    
    // set the new state if success and new sate changed
    if(res == STATUS_SUCCESS && biodyNewStateTemp != BIOCOM_NONE)
        BIOMSGM_biodyCurrentComState = biodyNewStateTemp;
    
    return res;
}

int BIOMSGM_manageMeasuresAcknowledgement(void){
    
    // request without encryption and jamming
    // no response to send, just change the current status to END_OF_EXCHANGES
  
    return STATUS_SUCCESS;
}

int BIOMSGM_manageExchangeKeyRequest(uint8_t *message, uint8_t messageLength){
    // 'e' request
        
    uint8_t payloadSize = GET_PAYLOAD_SIZE_REQUEST(message);
    uint8_t i;

    // unjam the message
    JAM_unjamBuffer(&message[2], messageLength-BIOMSGM_HEADER_AND_FOOTER_SIZE, assembling_buffer, jamSequence);
    
    // decrypt the message
    ENCRYPT_decryptBuffer(assembling_buffer, payloadSize, masterCryptKey);
    
    // create exchange key with R info N0 and N2 (N1 inverted)
    for(i=0; i<4; i++){
        exchangeCryptKey[i] = assembling_buffer[i] ^ randomN1[3-i] ^ randomN0[i];
    }
    
    // create response 's' with status 0x00
    return BIOMSGM_sendStatusResponse(BIOSTATCD_SUCCESS);
}

int BIOMSGM_manageAssociationRequest(uint8_t *message, uint8_t messageLength){
    // 'a'
    // get payloadSize
    uint8_t payloadSize = GET_PAYLOAD_SIZE_REQUEST(message);
    uint8_t jammingSize;;
    uint8_t i;
    uint8_t totalSize;
    uint16_t calculatedCrc;

    // get the N0 info from frame and store it / generate N1
    for(i=0; i<4; i++){
        randomN0[i] = message[PAYLOAD_START_INDEX+i];
        randomN1[i] = RAND_getRandomByte();
        assembling_buffer[i] = randomN1[i];
    }
    
    // create response 'b' with crypted by Cm, containing randomSequence
    messageRemainingTosend_buffer[0] = BIOCMD_ASSOCIATION_RESP;       // cmd, directly in the sending buffer
    
    // payload - Random Sequence crypted by exchange key
    payloadSize = 4;
    messageRemainingTosend_buffer[1] = payloadSize;
    ENCRYPT_encryptBuffer(assembling_buffer, payloadSize, masterCryptKey);
    
    // jam payload
    JAM_jamBuffer(assembling_buffer, payloadSize, &messageRemainingTosend_buffer[2], jamSequence);
    jammingSize = payloadSize/jamSequence;  
    totalSize =  payloadSize + BIOMSGM_HEADER_SIZE + jammingSize;        // final size for all message response without crc
    
    // calculate CRC and add it
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;
    
    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
}

int BIOMSGM_manageInformationRequest(uint8_t *message, uint8_t messageLength){
    
    // request without encryption and jamming
    uint16_t calculatedCrc;
    uint8_t payloadSize, i;
    uint8_t jammingSize;            //  bytes added by jamming
    uint8_t index, totalSize;

    uint8_t DEVICE_MODEL_NAME[] = {'B', 'I', 'O', 'D', 'Y'};

    BIOMSGM_loadPreviousExchangeKey(exchangeCryptKey);
    
    // CRC Ok, prepare and send the response 'j'
    messageRemainingTosend_buffer[0] = BIOCMD_INFO_RESP;       // cmd, directly in the sending buffer
    
    // PAYLOAD
    // FW version
    assembling_buffer[0] = FW_VERSION;
    assembling_buffer[1] = 0x00;       // end of FW version
    
    // device model
    for(index=2; index<DEVICE_MODEL_NAME_SIZE+2; index++)
        assembling_buffer[index] = DEVICE_MODEL_NAME[index-2];
    
    assembling_buffer[index] = 0x00;       // end of device model
    index++;
    
    // serial number in ascii
    for(i=0; i<SERIAL_NUMBER_MEM_SIZE; i++){
        //assembling_buffer[index] = Read_b_eep(SERIAL_NUMBER_MEM_ADDRESS + i) + 0x30;
        index++;
    }    
    assembling_buffer[index] = 0x00;       // end of serial number
    index++;
    
    // paylaoad size
    payloadSize = index; 
    messageRemainingTosend_buffer[1] = payloadSize;   
    
    // add encryption
    ENCRYPT_encryptBuffer(assembling_buffer, payloadSize, exchangeCryptKey);    // just crypt the payload
        
    // add jamming
    JAM_jamBuffer(assembling_buffer, payloadSize, &messageRemainingTosend_buffer[BIOMSGM_HEADER_SIZE], jamSequence);       // 4 correct bytes before a noise etc..., set the jamm payload with header in seding_buffer
    jammingSize = payloadSize/jamSequence;
    totalSize =  payloadSize + BIOMSGM_HEADER_SIZE + jammingSize;        // final size for all message response without crc
    
    // add Crc16
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;
        
    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
}

int BIOMSGM_manageBuildInformationRequest(uint8_t *message, uint8_t messageLength){
    
    uint8_t i, payloadSize, jammingSize, totalSize;
    uint16_t calculatedCrc;
    uint8_t index = 0;
    char* buildType = "0";	// BUILD_TYPE;
    char* buildDate = "28-01-19";	//BUILD_DATE;
    char* buildTime = "12h37";//BUILD_TIME;
    char* gitHash = "b'"; // BUILD_GIT_HASH;
    
    BIOMSGM_loadPreviousExchangeKey(exchangeCryptKey);
    
    // prepare the response 'l'
    messageRemainingTosend_buffer[0] = BIOCMD_DEV_INFO_RESP;
    
    // payload content in assembling buffer
    // build type
    assembling_buffer[index] = buildType[0];
    index++;
    
    // build date
    for(i=0; i<8; i++){
        if(buildDate[i] != '-'){
            assembling_buffer[index] = buildDate[i];
            index++;
        }
    }
 
    // build time
    for(i=0; i<5; i++){
        if(buildTime[i] != 'h'){
            assembling_buffer[index] = buildTime[i];
            index++;
        }
    }

    // git hash (10 st char)
    for(i=0; i<10; i++) {
        assembling_buffer[index] = gitHash[2+i];
        index++;
    }   
    
    // paylaoad size
    payloadSize = index;
    messageRemainingTosend_buffer[1] = payloadSize;
    
    // add encryption
    ENCRYPT_encryptBuffer(assembling_buffer, payloadSize, exchangeCryptKey);    // just crypt the payload
        
    // add jamming
    JAM_jamBuffer(assembling_buffer, payloadSize, &messageRemainingTosend_buffer[BIOMSGM_HEADER_SIZE], jamSequence);       // 4 correct bytes before a noise etc..., set the jamm payload with header in seding_buffer
    jammingSize = payloadSize/jamSequence;
    totalSize =  payloadSize + BIOMSGM_HEADER_SIZE + jammingSize;        // final size for all message response without crc
    
    // add Crc16
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;
    
    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
} 

// modifié par Marilys L
int BIOMSGM_manageMeasuresRequest(uint8_t *message, uint8_t messageLength){
    // request without jamming or encryption !
    
    uint16_t calculatedCrc;
    uint8_t payloadSize;
    uint8_t jammingSize, totalSize;

    // CRC Ok, prepare and send the response 'r'
    messageRemainingTosend_buffer[0] = BIOCMD_MEASURES_RESP;       // cmd, directly in the sending buffer
    
      // add the battery info + measures
        assembling_buffer[0] = NiveauVBAT;

        // measures
        assembling_buffer[1] = (0x00 | TableMesureGrandeursZPHI1[M5k][MZ]>>8);
        assembling_buffer[2] = TableMesureGrandeursZPHI1[M5k][MZ];
        assembling_buffer[3] = (0x10 | TableMesureGrandeursZPHI1[M5k][MPHI]>>8);
        assembling_buffer[4] = TableMesureGrandeursZPHI1[M5k][MPHI];

        assembling_buffer[5] = (0x00 | TableMesureGrandeursZPHI1[M20k][MZ]>>8);
        assembling_buffer[6] = TableMesureGrandeursZPHI1[M20k][MZ];
        assembling_buffer[7] = (0x10 | TableMesureGrandeursZPHI1[M20k][MPHI]>>8);
        assembling_buffer[8] = TableMesureGrandeursZPHI1[M20k][MPHI];

        assembling_buffer[9] = (0x00 | TableMesureGrandeursZPHI1[M50k][MZ]>>8);
        assembling_buffer[10] = TableMesureGrandeursZPHI1[M50k][MZ];
        assembling_buffer[11] = (0x10 | TableMesureGrandeursZPHI1[M50k][MPHI]>>8);
        assembling_buffer[12] = TableMesureGrandeursZPHI1[M50k][MPHI];

        assembling_buffer[13] = (0x00 | TableMesureGrandeursZPHI1[M100k][MZ]>>8);
        assembling_buffer[14] = TableMesureGrandeursZPHI1[M100k][MZ];
        assembling_buffer[15] = (0x10 | TableMesureGrandeursZPHI1[M100k][MPHI]>>8);
        assembling_buffer[16] = TableMesureGrandeursZPHI1[M100k][MPHI];

        assembling_buffer[17] = (0x00 | TableMesureGrandeursZPHI1[M200k][MZ]>>8);
        assembling_buffer[18] = TableMesureGrandeursZPHI1[M200k][MZ];
        assembling_buffer[19] = (0x10 | TableMesureGrandeursZPHI1[M200k][MPHI]>>8);
        assembling_buffer[20] = TableMesureGrandeursZPHI1[M200k][MPHI];

    payloadSize = 21;
    messageRemainingTosend_buffer[1] = payloadSize;
    
    // add encryption
    ENCRYPT_encryptBuffer(assembling_buffer, payloadSize, transferCryptKey);    // just crypt the payload

    // add jamming
    JAM_jamBuffer(assembling_buffer, payloadSize, &messageRemainingTosend_buffer[BIOMSGM_HEADER_SIZE], jamSequence);       // 4 correct bytes before a noise etc..., set the jamm payload with header in seding_buffer
    jammingSize = payloadSize/jamSequence;
    totalSize =  payloadSize + BIOMSGM_HEADER_SIZE + jammingSize;        // final size for all message response without crc

    // add Crc16
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;

    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
}

int BIOMSGM_manageTransferKeyRequest(uint8_t *message, uint8_t messageLength){
    //'c'
    uint8_t i;
    uint8_t payloadSize = GET_PAYLOAD_SIZE_REQUEST(message);
    uint8_t jammingSize;
    uint8_t randomSequenceNa[4], randomSequenceNb[4], totalSize;
    uint16_t calculatedCrc;

    // if current state not associated, load previous exchange key !
    if(BIOMSGM_biodyCurrentComState != BIOCOM_ASSOCIATED)
       BIOMSGM_loadPreviousExchangeKey(exchangeCryptKey);
    
    // unjam the message
    JAM_unjamBuffer(&message[2], messageLength-BIOMSGM_HEADER_AND_FOOTER_SIZE, assembling_buffer, jamSequence);
    
    // decrypt the message
    ENCRYPT_decryptBuffer(assembling_buffer, payloadSize, exchangeCryptKey);
    
    for(i=0; i<4; i++){
        // get Na [0-3]
        randomSequenceNa[i] = assembling_buffer[i];
        // create randomSequenceNb
        randomSequenceNb[i] = RAND_getRandomByte();
        // create the challenge C0
        challengeC0[i] = RAND_getRandomByte();
    }
    // create the transfer crypt key
    BIOMSGM_createTransferCryptKey(randomSequenceNa, randomSequenceNb, transferCryptKey);
 
    // prepare response 'd'
    messageRemainingTosend_buffer[0] = BIOCMD_CRYPT_KEY_RESP;       // cmd, directly in the sending buffer
    
    // payload - Nb et C0 crypted by exchange key
    for(i=0; i<4; i++)
        assembling_buffer[i] = randomSequenceNb[i];
    for(i=4; i<8; i++)
        assembling_buffer[i] = challengeC0[i-4];
    
    payloadSize = 8;
    messageRemainingTosend_buffer[1] = payloadSize;
    ENCRYPT_encryptBuffer(assembling_buffer, payloadSize, exchangeCryptKey);
    
    // jam payload
    JAM_jamBuffer(assembling_buffer, payloadSize, &messageRemainingTosend_buffer[2], jamSequence);
    jammingSize = payloadSize/jamSequence;  
    totalSize =  payloadSize + BIOMSGM_HEADER_SIZE + jammingSize;        // final size for all message response without crc
    
    // calculate CRC and add it
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;
    
    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
}

int BIOMSGM_manageTransferKeyVerifRequest(uint8_t *message, uint8_t messageLength){
    //'t'
    uint8_t i;
    uint8_t payloadSize = GET_PAYLOAD_SIZE_REQUEST(message);
    uint8_t jammingSize;
    uint8_t challengeTemp[4], totalSize;
    uint16_t calculatedCrc;

    // unjam the message
    JAM_unjamBuffer(&message[2], messageLength-BIOMSGM_HEADER_AND_FOOTER_SIZE, assembling_buffer, jamSequence);
    
    // decrypt the message
    ENCRYPT_decryptBuffer(assembling_buffer, payloadSize, transferCryptKey);
    
    // get C0 from frame and check if C0 is correct
    for(i=0; i<4; i++){
        // if not the same, wrong Ce
        if(assembling_buffer[i] != challengeC0[i]){
            return BIOSTATUS_EXCHANGE_KEY_ERROR;
        }           
    }
    
    // if C0 is correct, save the exchange key and send C1
    // if state is associated only !
    if(BIOMSGM_biodyCurrentComState == BIOCOM_ASSOCIATED){
        //BIOMSGM_saveExchangeKey(&exchangeCryptKey);
    }
    
    for(i=0; i<4; i++){
        // get C1 from frame
        challengeTemp[i] = assembling_buffer[4+i];
    }
 
    // prepare response 'u'
    messageRemainingTosend_buffer[0] = BIOCMD_TRANSFER_KEY_VERIF_RESP;       // cmd, directly in the sending buffer
    
    // payload - C1 crypted by transfer key
    for(i=0; i<4; i++)
        assembling_buffer[i] = challengeTemp[i];
    
    payloadSize = 4;
    messageRemainingTosend_buffer[1] = payloadSize;
    ENCRYPT_encryptBuffer(assembling_buffer, payloadSize, transferCryptKey);
    
    // jam payload
    JAM_jamBuffer(assembling_buffer, payloadSize, &messageRemainingTosend_buffer[2], jamSequence);
    jammingSize = payloadSize/jamSequence;  
    totalSize =  payloadSize + BIOMSGM_HEADER_SIZE + jammingSize;        // final size for all message response without crc
    
    // calculate CRC and add it
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;
    
    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
}

int BIOMSGM_sendStatusResponse(uint8_t error){
    
    // prepare the message with the error    
    uint16_t calculatedCrc;
    uint8_t totalSize = 3;
 
    // create response 'e' 
    messageRemainingTosend_buffer[0] = BIOCMD_STATUS_RESP;       // cmd, directly in the sending buffer
    
    messageRemainingTosend_buffer[1] = 1;
    messageRemainingTosend_buffer[2] = error;
        
    // calculate CRC and add it
    calculatedCrc = CRC16_calculateCrc16Xmodem(messageRemainingTosend_buffer, totalSize);
    messageRemainingTosend_buffer[totalSize] = calculatedCrc >> 8;
    messageRemainingTosend_buffer[totalSize+1] = calculatedCrc;
    
    return BIOMSGM_sendMessage(messageRemainingTosend_buffer, totalSize + BIOMSGM_FOOTER_SIZE);
}

int BIOMSGM_verifyCrc16(uint8_t *message, uint8_t messageLength){
    // calculate the crc of the message
    uint16_t calculatedCrc = CRC16_calculateCrc16Xmodem(message, messageLength-BIOMSGM_FOOTER_SIZE);
    
    // verify the CRC16
    if(calculatedCrc != (GET_CRC16_REQUEST(message, messageLength-BIOMSGM_FOOTER_SIZE))){
       return STATUS_SUCCESS;} //BIOSTATUS_CRC_ERROR;}
    
    return STATUS_SUCCESS;
}



int BIOMSGM_isValidBiodyMessage(uint8_t *message, uint8_t messageLength){
    

    uint8_t lengthExpected, res;
    uint8_t jammingSize = 0;

    // check the message length
    if(messageLength < 4){
        return BIOSTATUS_FRAME_ERROR;}

    // check the header > 0x61 <0x7A ('a' -> 'z')
    if(message[0] < 0x61 && message[0] > 0x7A){
        return BIOSTATUS_FRAME_ERROR;}
    
    // if cmd 'a' skip the jamming size
    if(message[0] != BIOCMD_ASSOCIATION_REQ){
        // check the length {
        jammingSize = GET_PAYLOAD_SIZE_REQUEST(message)/jamSequence;
    }
    lengthExpected = GET_PAYLOAD_SIZE_REQUEST(message) + BIOMSGM_HEADER_AND_FOOTER_SIZE + jammingSize;
    if(lengthExpected != messageLength){
        return BIOSTATUS_FRAME_ERROR;}
    
    // check the crc
    res = BIOMSGM_verifyCrc16(message, messageLength);
    if(res != STATUS_SUCCESS){
    	return BIOSTATUS_CRC_ERROR;}
    
    return STATUS_SUCCESS;    
}

int BIOMSGM_sendMessage(uint8_t *message, uint8_t messageSize){

    return BLEMSM_manageLeGattServerCharValIndicationReq(message, messageSize);
}

void BIOMSGM_saveExchangeKey(uint8_t *key){
    
    uint8_t i;
    // store the device id with the exchange key
    for (i = 0; i < DEVICE_ID_MEM_SIZE; i++)
       Write_b_eep(DEVICE_ID_STORED_MEM_ADDRESS + i, BLEMSM_deviceId[i]);
    for (i = 0; i < EXCHANGE_KEY_MEM_SIZE; i++)
        Write_b_eep(EXCHANGE_KEY_MEM_ADDRESS + i, key[i]);
}

void BIOMSGM_loadPreviousExchangeKey(uint8_t *destKey){
    
   // uint8_t i;
    //for(i=0; i<EXCHANGE_KEY_MEM_SIZE; i++)
        //destKey[i] = Read_b_eep(EXCHANGE_KEY_MEM_ADDRESS + i);
}

void BIOMSGM_createTransferCryptKey(uint8_t *na, uint8_t *nb, uint8_t *destKey){

	uint8_t i, index;
	// combine the 3rd bit of na(high value) and nb(low value) 
    // to get a nb between 0-3 to start the encryption by this index
	index = (na[0] >> 2)<<1 | nb[0]>>2;

	for(i=0; i<4; i++){
		if(index>3)
			index = 0;
		destKey[i] = na[index] ^nb[index];
		index += 1;
	}
}

void BIOMSGM_loadMasterKey(uint8_t *destKey){
    
   uint8_t i;
   for(i=0; i<MASTER_KEY_MEM_SIZE; i++){
	   destKey[i] = DEFAULT_MASTER_CRYPT_KEY[i];
   }
}

void BIOMSGM_saveMasterKey(uint8_t *key){
    
    //uint8_t i;
    // store the master key
    //for (i = 0; i < MASTER_KEY_MEM_SIZE; i++)
        //Write_b_eep(MASTER_KEY_MEM_ADDRESS + i, key[i]);
}

void BIOMSGM_manageMasterKey(void){
    
    uint8_t i;
    // get key from eeprom
    BIOMSGM_loadMasterKey(masterCryptKey);
    
    // check the key
    for(i=1; i<MASTER_KEY_MEM_SIZE; i++){
        if(masterCryptKey[i] == masterCryptKey[i-1]){
            // if too many similarities, load a default key
            for(i=0; i<MASTER_KEY_MEM_SIZE; i++)
                masterCryptKey[i] = DEFAULT_MASTER_CRYPT_KEY[i];            
            // save the master key
            BIOMSGM_saveMasterKey(DEFAULT_MASTER_CRYPT_KEY);
            break;
        }           
    }
}
