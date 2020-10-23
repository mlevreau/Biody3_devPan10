/*
 * SPP_OVER_BLE.c
 *
 *  Created on: 24 Oct 2018
 *      Author: Adrien Mallet
 */

#include "spp_over_ble.h"
#include "stdint.h"
#include "communication.h"
#include "status.h"
#include "buzzer.h"
#include "pconfig.h"
#include "const.h"
#include "trace.h"
#include "uart_hal.h"
#include "tcu.h"
#include "global_buffer.h"
#include "ble_tcu_commands.h"
#include "ble_message_manager.h"
#include "led.h"
#include "version.h"
#include "ble_biody_profile_data_cmds.h"

// --- Protocol section ---
#include "biody_message_manager.h"

#include "global.h"
#include "delays_sleep.h"

#undef MODULE_NAME
#define MODULE_NAME "SPPOBLE"

#define BLE_CONNECTION_COMPLETE ((GET_SERVICE_ID == TCU_LE_SERVICE_MNG) && (GET_OP_CODE == TCU_MNG_LE_CONNECTION_COMPLETE_EVENT))
#define BLE_WRITE_COMPLETE ((GET_SERVICE_ID == TCU_LE_SERVICE_GATT_SRV) && (GET_OP_CODE == TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT))
#define BLE_INDICATIONS_ASKED ((GET_SERVICE_ID == TCU_LE_SERVICE_GATT_SRV) && (GET_OP_CODE == TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT))

#define BLE_CONNECTION_TIMEOUT 30      

    // init timeouts
#if DEV_TEST
    #define BLE_MESSAGE_RECEPTION_TIMEOUT 20
    #define BLE_EXCHANGES_TIMEOUT 12
#else
    #define BLE_MESSAGE_RECEPTION_TIMEOUT 5
    #define BLE_EXCHANGES_TIMEOUT 15
#endif

enum communicationState currentCommunicationState = COM_NONE;

uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4[24];
uint8_t TCU_HCI_WRITE_BD_ADDR_REQ[10];
uint8_t TCU_MNG_LE_START_ADVERTISE_REQ_ARR[86];

// info of biody type, used to recognized a specific type of Biody before connection
uint8_t BIODY_SYSTEM_TYPE = 0x01;

int SPPOBLE_handleState(enum communicationState newState){

    int res = STATUS_SUCCESS;
    
	if(newState == currentCommunicationState)
        return STATUS_SUCCESS;
    
    // apply the new state at the current
    currentCommunicationState = newState;
	
    trace_info(MODULE_NAME, "( handleState ) current state %d / new state %d", currentCommunicationState, newState);

	switch(currentCommunicationState){
		case COM_INIT:   
	    	PRINT(rs_ctx,"COM_INIT \n");
			break;
		case COM_UART_INIT:
	    	PRINT(rs_ctx,"COM_UART_INIT \n");
			//UART_init(); done in main.c
			break;
		case COM_PROFILE_INIT:
	    	PRINT(rs_ctx,"COM_PROFILE_INIT \n");
            res = SPPOBLE_initProfile();
			break;
		case COM_WAITING_BT_CONNECTION:
	    	PRINT(rs_ctx,"COM_WAITING_BT_CONNECTION \n");
            res = SPPOBLE_waitForConnection(BLE_CONNECTION_TIMEOUT);   
			break;
		case COM_BT_CONNECTED:
	    	PRINT(rs_ctx,"COM_BT_CONNECTED \n");
			// disable BLE
			break;
		case COM_BLE_CONNECTED:
	    	PRINT(rs_ctx,"COM_BLE_CONNECTED \n");
            BIOMSGM_manageMasterKey();  // init and check the crypt key 
			break;
		case COM_WAITING_EXCHANGE:       
	    	PRINT(rs_ctx,"COM_WAITING_EXCHANGE \n");
            res = SPPOBLE_waitForExchange(BLE_MESSAGE_RECEPTION_TIMEOUT);      
			break;
		case COM_EXCHANGE_IN_PROGRESS:
	    	PRINT(rs_ctx,"COM_EXCHANGE_IN_PROGRESS \n");
            res = SPPOBLE_manageExchanges(BLE_EXCHANGES_TIMEOUT);      
			break;
		case COM_EXCHANGE_DONE:
	    	PRINT(rs_ctx,"COM_EXCHANGE_DONE \n");
			// call the disconnection (BT or BLE) ?
			break;
		case COM_DISCONNECTED:
	    	PRINT(rs_ctx,"COM_DISCONNECTED \n");
			// finalize the com process and leave ?
			break;

		default:
	    	PRINT(rs_ctx,"Case SPPOBLE_handleState default -> break \n");
			break;
	}
    
    // manage timeout 
    if(res == STATUS_TIMEOUT){
        // send a timeout reached status to the device
        SPPOBLE_manageCommunicationError(BIOSTATUS_TIMEOUT_REACHED_ERROR);
        // wait for msg to be sent before returning
        while(UART_isDataToSendWaiting());
    }
	if (res)
		return STATUS_ERROR;		
	else
		return STATUS_SUCCESS;
}

int SPPOBLE_initProfile(void){
    
    int res;
    
    res = COM_initHci();
	PRINT(rs_ctx,"(initProfile) COM_initHci: %s\n",(res==0)?"SUCCESS":"ERROR");
	if(res != STATUS_SUCCESS)
		return res;
    
    // update profile data with information from Hci init
    SPPOBLE_updateProfileData();

	res = COM_initBLE();
	if(res != STATUS_SUCCESS)
		return res;

	res = COM_initSppOverBLE();
	if(res != STATUS_SUCCESS)
		return res;
    
    res = COM_startToAdvertise();
    if(res != STATUS_SUCCESS)
        return res;
    
    return STATUS_SUCCESS;
}

int SPPOBLE_createProfile(void){
    
    int res;
            
    res = SPPOBLE_handleState(COM_PROFILE_INIT);
	PRINT(rs_ctx,"SPPOBLE_handleState: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS){
        return res;
    }
    return STATUS_SUCCESS;
}

void SPPOBLE_updateProfileData(void){

    uint8_t i;
    uint8_t* updatePtr;     // pointer for array to update
        
    // ---------------- update the SYSTEM ID info
    updatePtr = &TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4[SYSTEM_ID_CHAR_DATA_START_INDEX];
    // update data
    updatePtr[0] = BIODY_SYSTEM_TYPE;  
    // add the info to SYSTEM_ID charac
    updatePtr = &TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4[SYSTEM_ID_CHAR_DATA_START_INDEX];
    for(i=0; i<SPPOBLE_BD_ADDR_MINIMIZE_LENGTH; i++)
        updatePtr[1+i] = TCU_HCI_WRITE_BD_ADDR_REQ[7-i];
        
    // -------------- update the Adv data 
    // add the custom data after complete local name (BIODY)
    updatePtr = &TCU_MNG_LE_START_ADVERTISE_REQ_ARR[ADVERTISEMENT_DATA_CHAR_CUSTOM_DATA_START_INDEX];
    updatePtr[0] = 1 + 5;   // 1 = data type + data length
    updatePtr[1] = 0xFF;       // see https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile 
    for(i=0; i<5; i++)
        updatePtr[2+i] = (&TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4[SYSTEM_ID_CHAR_DATA_START_INDEX])[i];
    // update the total length for adv data
    ADVERTISEMENT_DATA_CHAR_DATA_LENGTH == ADVERTISEMENT_DATA_CHAR_DATA_LENGTH + ADVERTISEMENT_DATA_CHAR_CUSTOM_DATA_START_INDEX + 1; // +1 for data length in adv custom data
}

int SPPOBLE_waitForConnection(uint8_t timeout){
    int res;
    
    unsigned int volatile TimeOUT2 = 0;

    // convert timeout in ms for the comparaison
    while(TimeOUT2*10 < timeout*1000){
        // check if data available !
        res = UART_checkDataAvailable(200);
       // PRINT(rs_ctx,"(waitConnection)UART_checkDataAvailable: %s\n",(res==0)?"SUCCESS":"ERROR");
        // if data available, test if it's a connection request !!
        if(res == STATUS_SUCCESS){
            res = BLEMSM_handleBleMessage();
            if(res != STATUS_SUCCESS)
                return res;
            
            // check if correct Service id and op code 
            if(BLE_CONNECTION_COMPLETE)
                return STATUS_SUCCESS;
        }   
    }
    return STATUS_TIMEOUT;
}

int SPPOBLE_waitForExchange(uint8_t timeout){
    int res;
    
    unsigned int volatile TimeOUT2 = 0;
    
    // convert timeout in ms for the comparaison
    while(TimeOUT2*10 < timeout*1000){
        // check if data available !
        res = UART_checkDataAvailable(200);                 
        if(res == STATUS_SUCCESS){
            res = BLEMSM_handleBleMessage();
            if(res != STATUS_SUCCESS)
                return res;
            
            // check if correct Service id and op code 
            if(BLE_WRITE_COMPLETE || BLE_INDICATIONS_ASKED)
                return STATUS_SUCCESS;
        }   
    }
    return STATUS_TIMEOUT;
}

int SPPOBLE_manageExchanges(uint8_t timeout){
    
    int res;
    unsigned int volatile TimeOUT2 = 0;
    
    // convert timeout in ms for the comparaison
    while(TimeOUT2*10 < timeout*1000){

        // check if data available !
        res = UART_checkDataAvailable(100);
        PRINT(rs_ctx,"(manageExchanges)UART_checkDataAvailable: %s\n",(res==0)?"SUCCESS":"ERROR");
        // If end of exchanges, 
        if(BIOMSGM_END_OF_EXCHANGES){
            PRINT(rs_ctx,"(manageExchanges)BIOMSGM_END_OF_EXCHANGES\n");
            return STATUS_SUCCESS;
        }
        if(res == STATUS_SUCCESS){
            PRINT(rs_ctx,"(manageExchanges)res == STATUS_SUCCESS\n");
            res = BLEMSM_handleBleMessage();
            PRINT(rs_ctx,"(manageExchanges)BLEMSM_handleBleMessage: %s\n",(res==0)?"SUCCESS":"ERROR");
            if(res != STATUS_SUCCESS){
                return res;
            }
           // res = STATUS_TIMEOUT;
        }   
        // manage the messages
        else if(res == STATUS_TIMEOUT){
            PRINT(rs_ctx,"(manageExchanges)If TIMEOUT\n");
            // manage the previous message packets if needed
           if(BLEMSM_previousMessagePacketToSend){
                PRINT(rs_ctx,"(manageExchanges)BLEMSM_previousMessagePacketToSend\n");
                res = SPPOBLE_managePreviousMessagePacketToSend(messageRemainingTosend_buffer, BLEMSM_previousMessagePacketToSendLength);
            }
            // or manage a received message 
            else if (BLEMSM_messageReceived){
                PRINT(rs_ctx,"(manageExchanges)BLEMSM_messageReceived\n");
            messageReceived_buffer [0] =	TableMesureGrandeursZPHI1[M5k][MZ];
            messageReceived_buffer [1] =    TableMesureGrandeursZPHI1[M20k][MZ];
            messageReceived_buffer [2] =	TableMesureGrandeursZPHI1[M50k][MZ];
            messageReceived_buffer [3] =	TableMesureGrandeursZPHI1[M100k][MZ];
            messageReceived_buffer [4] =	TableMesureGrandeursZPHI1[M200k][MZ];

            messageReceived_buffer [5] =	TableMesureGrandeursZPHI1[M5k][MPHI];
            messageReceived_buffer [6] =	TableMesureGrandeursZPHI1[M20k][MPHI];
            messageReceived_buffer [7] =	TableMesureGrandeursZPHI1[M50k][MPHI];
            messageReceived_buffer [8] =	TableMesureGrandeursZPHI1[M100k][MPHI];
            messageReceived_buffer [9] =	TableMesureGrandeursZPHI1[M200k][MPHI];

        	PRINT(rs_ctx,"BLEMSM_messageReceivedLength: %d\n",BLEMSM_messageReceivedLength);
                res = SPPOBLE_manageMessage(messageReceived_buffer, BLEMSM_messageReceivedLength);

                // Manage all errors cases
                if (res != STATUS_SUCCESS){
                    PRINT(rs_ctx,"(manageExchanges)res != STATUS_SUCCESS\n");
                    SPPOBLE_manageCommunicationError(res);
                }
            }
        }
    }
    return STATUS_TIMEOUT;
}

int SPPOBLE_manageMessage(uint8_t *message, uint8_t messageLength){
    
    // clear data and flag
    BLEMSM_messageReceived = 0;
    BLEMSM_messageReceivedLength = 0;

	PRINT(rs_ctx,"messageLength: %d\n",messageLength);
    return BIOMSGM_manageMessage(message, messageLength);
    
}

int SPPOBLE_managePreviousMessagePacketToSend(uint8_t *messageRemaining, uint8_t messageRemainingLength){
        
    // clear flag and data
    BLEMSM_previousMessagePacketToSend = 0;
    BLEMSM_previousMessagePacketToSendLength = 0;
    
    // move the pointer to next packet  WARNING !! verify the buffer size max packets !!!!
    messageRemaining += BLE_MAX_PACKET_SIZE;
    
    return BLEMSM_manageLeGattServerCharValIndicationReq(messageRemaining, messageRemainingLength);
}

int SPPOBLE_manageProfile(void){
    
    int res;
    // start by the creation of the profile
    res = SPPOBLE_createProfile();
	PRINT(rs_ctx,"SPPOBLE_createProfile: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;    
    
    res = SPPOBLE_handleState(COM_WAITING_BT_CONNECTION);
	PRINT(rs_ctx,"COM_WAITING_BT_CONNECTION: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    res = SPPOBLE_handleState(COM_BLE_CONNECTED);
	PRINT(rs_ctx,"COM_WAITING_BT_CONNECTION: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    res = SPPOBLE_handleState(COM_WAITING_EXCHANGE);
	PRINT(rs_ctx,"COM_WAITING_EXCHANGE: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    //not used in marilys test
    // change the beeps frequency when first exchange received
   // BUZZ_stopBuzzer();
   // BUZZ_startEchangesInProgressBeeps();
    
    res = SPPOBLE_handleState(COM_EXCHANGE_IN_PROGRESS);
	PRINT(rs_ctx,"COM_EXCHANGE_IN_PROGRESS: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    res = SPPOBLE_handleState(COM_EXCHANGE_DONE);
	PRINT(rs_ctx,"COM_EXCHANGE_DONE: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    res = SPPOBLE_handleState(COM_DISCONNECTED);
	PRINT(rs_ctx,"COM_DISCONNECTED: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    // END OF EXCHANGES
    return STATUS_SUCCESS;
}

void SPPOBLE_manageCommunicationError(uint8_t error){
   
    switch(error){
        case BIOSTATUS_FRAME_ERROR:
            BIOMSGM_sendStatusResponse(BIOSTATCD_WRONG_FRAME);
            break;
        case BIOSTATUS_CRC_ERROR:
            BIOMSGM_sendStatusResponse(BIOSTATCD_WRONG_CRC);
            break;
        case BIOSTATUS_PROCESS_ERROR:
            BIOMSGM_sendStatusResponse(BIOSTATCD_PROCESS_FAILED);
            break;
        case BIOSTATUS_EXCHANGE_KEY_ERROR:
            BIOMSGM_sendStatusResponse(BIOSTATCD_WRONG_EXCHANGE_KEY);
            break;
        case BIOSTATUS_TIMEOUT_REACHED_ERROR:
            BIOMSGM_sendStatusResponse(BIOSTATCD_TIMEOUT_REACHED);
            break;
        default:
            break;
    }
}


