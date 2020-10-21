/*
 * BLE_MESSAGE_MANAGER.c
 *
 *  Created on: 25 Oct 2018
 *      Author: Adrien Mallet
 */

#include <string.h>

#include "ble_message_manager.h"
#include "status.h"
#include "ble_tcu_commands.h"
#include "global_buffer.h"
#include "const.h"
#include "global.h"
#include "tcu.h"
#include "trace.h"
#include "ble_biody_profile_data_cmds.h"
//#include "delays_sleep.h"
#include "version.h"


#undef MODULE_NAME
#define MODULE_NAME "BLEMSM"


int BLEMSM_handleBleMessage(void){
    int res;
    uint8_t serviceId;
    
    // retrieve the message via TCU protocol
    res = TCU_receiveResponse(reception_buffer, 0);
    PRINT(rs_ctx,"(handleMessage)TCU_receiveResponse: %s\n",(res==0)?"SUCCESS":"ERROR");
    if(res != STATUS_SUCCESS)
        return res;
    
    // get service id
    serviceId = GET_SERVICE_ID;
   // PRINT(rs_ctx,"serviceId: %d\n",serviceId);
    
    // switch for service id
    switch (serviceId) {
        case TCU_BT_SERVICE_MNG:
        	PRINT(rs_ctx,"TCU_BT_SERVICE_MNG\n");
            return STATUS_SUCCESS;
        case TCU_BT_SERVICE_SPP:
        	PRINT(rs_ctx,"TCU_BT_SERVICE_SPP\n");
            return STATUS_SUCCESS;
        case TCU_LE_SERVICE_MNG:            // D1 (Connection, req ok(indication, )
        	PRINT(rs_ctx,"BLEMSM_manageLeServiceManager\n");
            return BLEMSM_manageLeServiceManager();
        case TCU_LE_SERVICE_GATT_CLI:
        	PRINT(rs_ctx,"TCU_LE_SERVICE_GATT_CLI\n");
            return STATUS_SUCCESS;
        case TCU_LE_SERVICE_GATT_SRV:       // D3 (write charac, indications, )
        	PRINT(rs_ctx,"BLEMSM_manageLeServiceGattServer\n");
            return BLEMSM_manageLeServiceGattServer();
        case TCU_LE_SERVICE_SMP_SLV:
        	PRINT(rs_ctx,"TCU_LE_SERVICE_SMP_SLV\n");
            return STATUS_SUCCESS;
        case TCU_LE_SERVICE_SMP_MAS:
        	PRINT(rs_ctx,"TCU_LE_SERVICE_SMP_MAS\n");
            return STATUS_SUCCESS;
        default:
            trace_error(MODULE_NAME, "Service Id unknown");
            break;
    }
    
    return STATUS_SUCCESS;
}

//************************* D1 LE SERVICE MANAGER ***************************

int BLEMSM_manageLeServiceManager(void){
    
    switch(GET_OP_CODE){
        case TCU_MNG_LE_CONNECTION_COMPLETE_EVENT:      // 1st co
        	PRINT(rs_ctx,"BLEMSM_manageLeConnectionCompleteEvent\n");
            return BLEMSM_manageLeConnectionCompleteEvent();
        case TCU_MNG_LE_CONNECTION_UPDATE_EVENT:
        	PRINT(rs_ctx,"BLEMSM_manageLeConnectionUpdateEvent\n");
            return BLEMSM_manageLeConnectionUpdateEvent();  
        case TCU_LE_ACCEPT:
        	PRINT(rs_ctx,"BLEMSM_manageLeAccept\n");
            return BLEMSM_manageLeAccept();
        case TCU_MNG_LE_DISCONNECT_EVENT:
        	PRINT(rs_ctx,"BLEMSM_manageLeDisconnectEvent\n");
            return BLEMSM_manageLeDisconnectEvent();
        default:
            return STATUS_SUCCESS;         
    }
}

int BLEMSM_manageLeConnectionCompleteEvent(void){

    // get useful connection info for the next exchanges
    BLEMSM_connectionHandle[0] = reception_buffer[8];
    BLEMSM_connectionHandle[1] = reception_buffer[9];
   //Role = reception_buffer[10];
    //show_data(BLEMSM_connectionHandle);

    
    // store the peer address of the device (phone etc...)
    BLEMSM_deviceId[0] = reception_buffer[17];
    BLEMSM_deviceId[1] = reception_buffer[16];
    BLEMSM_deviceId[2] = reception_buffer[15];
    BLEMSM_deviceId[3] = reception_buffer[14];
    BLEMSM_deviceId[4] = reception_buffer[13];
    BLEMSM_deviceId[5] = reception_buffer[12];
    
    show_data(BLEMSM_deviceId);

    if(TCU_STATUS_ERROR)
        return STATUS_ERROR;
    
    return STATUS_SUCCESS;
}

int BLEMSM_manageLeConnectionUpdateEvent(void){
    // do nothing for now
    return STATUS_SUCCESS;
}

int BLEMSM_manageLeAccept(void){
    
    if(TCU_STATUS_ERROR)
        return STATUS_ERROR;
    
    return STATUS_SUCCESS;
}

int BLEMSM_manageLeDisconnectEvent(void){
    
    // disconnection event
    if(GET_DISCONNECTION_REASON(reception_buffer) == 0x13)          // remote user terminated connection
        return STATUS_SUCCESS;
    else if(GET_DISCONNECTION_REASON(reception_buffer) == 0x16)     // connection terminated by local host
        return STATUS_SUCCESS;
    
    return STATUS_ERROR;
}

// *********************** D3 LE SERVICE GATT SERVER ************************
int BLEMSM_manageLeServiceGattServer(void){
    
    switch(GET_OP_CODE){
        case TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT:      // write
        	//PRINT(rs_ctx,"TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT\n");
            return BLEMSM_manageLeGattServerWriteCharValueEvent();
        case TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT:
        	//PRINT(rs_ctx,"TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT\n");
         //   return BLEMSM_manageLeGattServerReadCharValueEvent();
        case TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP:
        //	PRINT(rs_ctx,"TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP\n");
        //    return BLEMSM_manageLeGattSdbUpdateCharEleResponse();
        case TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_RESP:
        case TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_RESP:
        case TCU_LE_GATT_SER_CHAR_VAL_INDICATION_EVENT:
        case TCU_LE_GATT_SER_EXG_MTU_ACCEPT_RESP:               // see if needed to store MTU
        case TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RESP:
        case TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_RESP:        // see if response needed !!
        case TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP:
        //	PRINT(rs_ctx,"TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP\n");
         //   return BLEMSM_manageLeGattServerWriteCharValAcceptResp();
        case TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT:
        //	PRINT(rs_ctx,"TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT\n");
          //  return BLEMSM_manageLeGattServerWriteCharDespEvent();
        case TCU_LE_GATT_SER_EXG_MTU_EVENT:
        //	PRINT(rs_ctx,"TCU_LE_GATT_SER_EXG_MTU_EVENT\n");
           // return BLEMSM_manageLeGattServerExgMtuEvent();
        case TCU_LE_GATT_SER_READ_MULTIPLE_EVENT:   // to see if correct
        //	PRINT(rs_ctx,"TCU_LE_GATT_SER_READ_MULTIPLE_EVENT\n");
           // return BLEMSM_manageLeGattServerReadMultipleEvent();
        case TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT:
       // 	PRINT(rs_ctx,"TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT\n");
           // return BLEMSM_manageLeGattServerReadCharDespEvent();
        default:
            return STATUS_SUCCESS;       
    }
}

int BLEMSM_manageLeGattServerWriteCharValueEvent(void){
    //get the length of data
    uint8_t i;
    uint16_t messageLength = GET_MSG_LENGTH(reception_buffer);
    
    // retrieve the charac value handle
    BLEMSM_characteristicValueHandle[0] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX];
    BLEMSM_characteristicValueHandle[1] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX + 1];
      
    // prepare the response / add the specific info  
    assembling_buffer[0] = BLEMSM_characteristicValueHandle[0];     // charac handle
    assembling_buffer[1] = BLEMSM_characteristicValueHandle[1];     // charac handle
    assembling_buffer[2] = messageLength;                   // message Legtn
    assembling_buffer[3] = 0;
   
    // add the message
    for(i=0; i<messageLength; i++){
        assembling_buffer[4+i] = reception_buffer[MESSAGE_START_INDEX + i];
        // store the message
        messageReceived_buffer[i] = reception_buffer[MESSAGE_START_INDEX + i];
    }
    // indicate message received
    BLEMSM_messageReceived = 1;
    BLEMSM_messageReceivedLength = messageLength;
    
    // update the attribute value for charac and desciptor
    return TCU_sendResponse(sending_buffer, assembling_buffer, (messageLength+4), TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ);
}

/*

int BLEMSM_manageLeGattServerReadCharValueEvent(void){
    
    uint8_t res, attributeLength;
    uint16_t characHandle;
    uint8_t* attributeArray;
    uint8_t i;
    
    assembling_buffer[0] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX];
    assembling_buffer[1] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX +1];
    
    characHandle = (assembling_buffer[1] << 8) | assembling_buffer[0];
    // switch attibute by handle
    switch(characHandle){
        case MANUFACTURER_NAME_CHAR_HANDLE_VALUE: // Manufacturer String
            attributeLength = MANUFACTURER_NAME_CHAR_DATA_LENGTH;
            attributeArray = &MANUFACTURER_NAME_CHAR_DATA_START_INDEX;    //get tab to point + index to start for copy
            break;
        case MODEL_NUMBER_CHAR_HANDLE_VALUE: // Model number String
            attributeLength = MODEL_NUMBER_CHAR_DATA_LENGTH;
            attributeArray = &MODEL_NUMBER_CHAR_DATA_START_INDEX;
            break;
        case SYSTEM_ID_CHAR_HANDLE_VALUE: // System ID
            // send an info from version.h for now
            attributeLength = SYSTEM_ID_CHAR_DATA_LENGTH;
            attributeArray = &SYSTEM_ID_CHAR_DATA_START_INDEX;
            break;
        case HARDWARE_REVISION_CHAR_HANDLE_VALUE:    // Hardware revision String
            attributeLength = HARDWARE_REVISION_CHAR_DATA_LENGTH;
            attributeArray = &HARDWARE_REVISION_CHAR_DATA_START_INDEX;
            break;
        case DEVICE_NAME_CHAR_HANDLE_VALUE:
            attributeLength = DEVICE_NAME_CHAR_DATA_LENGTH;
            attributeArray = &DEVICE_NAME_CHAR_DATA_START_INDEX;
            break; 
        default:
            attributeLength = 0;
            break;
    }
    
    assembling_buffer[2] = attributeLength;       // attribute length
    assembling_buffer[3] = 0x00;                   // attribute length
    
    // add the attribute value !
    for(i=0; i<attributeLength; i++){
        assembling_buffer[4+i] = attributeArray[i];
    }
    
    // update the attribute value for charac and desciptor
    res = TCU_sendResponse(sending_buffer, assembling_buffer, (attributeLength+4), TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ);
    
    if(res != STATUS_SUCCESS)
        return res;    
            
    // prepare the response TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RE
    BLEMSM_setResponseHeaderWithStatusSuccess(assembling_buffer);              // co handle + status success  
    assembling_buffer[3] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX];
    assembling_buffer[4] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX +1];
            
    return TCU_sendResponse(sending_buffer, assembling_buffer, 5, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ);
}

int BLEMSM_manageLeGattSdbUpdateCharEleResponse(void){
    
    // just check the status
    if(TCU_STATUS_ERROR)
        return STATUS_ERROR;
    
    // prepare the response
    BLEMSM_setResponseHeaderWithStatusSuccess(assembling_buffer);              // co handle + status success  
    assembling_buffer[3] = BLEMSM_characteristicValueHandle[0];    // error handle
    assembling_buffer[4] = BLEMSM_characteristicValueHandle[1];    // error handle
    
    // respond with TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ
    return TCU_sendResponse(sending_buffer, assembling_buffer, 5, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ);
}

int BLEMSM_manageLeGattServerWriteCharValAcceptResp(void){
    
    // just check the status
    if(TCU_RESP_STATUS_ERROR)
        return STATUS_ERROR;
    
    return STATUS_SUCCESS;
}

int BLEMSM_manageLeGattServerWriteCharDespEvent(void){
    
    // get useful infos
    BLEMSM_characteristicDescriptorHandle[0] = reception_buffer[TCU_HEADER_LENGTH+6];
    BLEMSM_characteristicDescriptorHandle[1] = reception_buffer[TCU_HEADER_LENGTH+7];
    BLEMSM_characteristicDescriptorValue[0] = reception_buffer[TCU_HEADER_LENGTH+8];
    BLEMSM_characteristicDescriptorValue[1] = reception_buffer[TCU_HEADER_LENGTH+9];
    
    
    // prepare response TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ
    BLEMSM_setResponseHeaderWithStatusSuccess(assembling_buffer);              // co handle + status success  
    assembling_buffer[3] = BLEMSM_characteristicDescriptorValue[0];    // error handle
    assembling_buffer[4] = BLEMSM_characteristicDescriptorValue[1];    // error handle
    
    return TCU_sendResponse(sending_buffer, assembling_buffer, 5, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ);
}

int BLEMSM_manageLeGattServerExgMtuEvent(void){
    
    // get server rx mtu size
    BLEMSM_serverRxMtuSize[0] = reception_buffer[TCU_HEADER_LENGTH+6];
    BLEMSM_serverRxMtuSize[1] = reception_buffer[TCU_HEADER_LENGTH+7];
    
    // just respond TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ
    BLEMSM_setResponseHeaderWithStatusSuccess(assembling_buffer);              // co handle + status success  
    assembling_buffer[3] = BLEMSM_serverRxMtuSize[0];
    assembling_buffer[4] = BLEMSM_serverRxMtuSize[1];
    
    return TCU_sendResponse(sending_buffer, assembling_buffer, 5, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ);
}

int BLEMSM_manageLeGattServerReadMultipleEvent(void){
    
    BLEMSM_setResponseHeaderWithStatusSuccess(assembling_buffer);              // co handle + status success  
    assembling_buffer[3] = reception_buffer[TCU_HEADER_LENGTH+6];                  
    assembling_buffer[4] = reception_buffer[TCU_HEADER_LENGTH+7];                    
    
    //send response TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_REQ
    return TCU_sendResponse(sending_buffer, assembling_buffer, 5, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_REQ);
}

int BLEMSM_manageLeGattServerReadCharDespEvent(void){
    
    int res, attributeLength;
    uint16_t characHandle;
    uint8_t* attributeArray;
    uint8_t i;
    
    assembling_buffer[0] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX];
    assembling_buffer[1] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX +1];
    
    characHandle = (assembling_buffer[1] << 8) | assembling_buffer[0];
    // switch attibute by handle
    switch(characHandle){
        case 0x0212:
            attributeLength = 0x02;
            attributeArray = &TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ6[14];
            break;
        default:
            attributeLength = 0;
            break;
    }
    
    assembling_buffer[2] = attributeLength;       // attribute length
    assembling_buffer[3] = 0x00;                   // attribute length
    
    // add the attribute value !
    for(i=0; i<attributeLength; i++){
        assembling_buffer[4+i] = attributeArray[i];
    }
    
    // update the attribute value for charac and desciptor
    res = TCU_sendResponse(sending_buffer, assembling_buffer, (attributeLength+4), TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ);
    
    if(res != STATUS_SUCCESS)
        return res;
    
    // prepare the response TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ
    BLEMSM_setResponseHeaderWithStatusSuccess(assembling_buffer);              // co handle + status success            
    assembling_buffer[3] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX];
    assembling_buffer[4] = reception_buffer[CHARC_VALUE_HANDLE_START_INDEX +1];
            
    return TCU_sendResponse(sending_buffer, assembling_buffer, 5, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_REQ);
}
*/
// ******************* Indications ***************************

int BLEMSM_manageLeGattServerCharValIndicationReq(uint8_t *message, uint8_t messageLength){

    // used to send indication values   //TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ
    uint8_t i;
    assembling_buffer[0] = BLEMSM_connectionHandle[0];                       // co handle
    assembling_buffer[1] = BLEMSM_connectionHandle[1];                       // co handle
    assembling_buffer[2] = SPPOBLE_CHAR_HANDLE_VALUE & 0xFF;        // handle fix
    assembling_buffer[3] = SPPOBLE_CHAR_HANDLE_VALUE >> 8 & 0xFF;

    // message size verification
    if(messageLength > BLE_MAX_PACKET_SIZE){
        // send the messsage left in an other packet
        BLEMSM_previousMessagePacketToSend = 1;
        BLEMSM_previousMessagePacketToSendLength = messageLength - BLE_MAX_PACKET_SIZE;
        
        // change the size for 1st packet
        messageLength = BLE_MAX_PACKET_SIZE;
    }
    
    // add values to send
    for(i=0; i<messageLength; i++){
        assembling_buffer[4+i] = message[i];
    }
    PRINT(rs_ctx,"assembling_buffer:\n");
    show_data(assembling_buffer);

    PRINT(rs_ctx,"sending_buffer: %d \n", sending_buffer);

    return TCU_sendResponse(sending_buffer, assembling_buffer, 4+messageLength, TCU_LE_SERVICE_GATT_SRV, TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ);
}

// ******************** METHODS ***************************
/*
void BLEMSM_setResponseHeaderWithStatusSuccess(uint8_t *dest){
    
    dest[0] = BLEMSM_connectionHandle[0];                           // co handle
    dest[1] = BLEMSM_connectionHandle[1];                           // co handle
    dest[2] = STATUS_SUCCESS;                          // status ok
}
*/
