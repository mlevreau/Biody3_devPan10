/*
 * communication.c
 *
 *  Created on: Jun 05, 2020
 *      Author: JYL
 *      Adapted from original development by Adrien Mallet
 */

#include <stdint.h>
#include "communication.h"
#include "uart.h"
#include "tim.h"
#include "global_buffer.h"
#include "hci.h"
#include "tcu.h"
#include "trace.h"
#include "status.h"
#include "ble_biody_profile_data_cmds.h"

#undef MODULE_NAME
#define MODULE_NAME "COM"

const uint8_t TCU_HCI_RESET_REQUEST[4] = {0x01, 0x03, 0x0C, 0x00};
const uint8_t TCU_HCI_GET_FW_VERSION_REQ[13] = {0x01, 0x08, 0xFC, 0x09, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x14, 0x0D, 0xFF, 0x00};
const uint8_t TCU_HCI_M2_BTL_SET_I2C_ENABLE_REQ[15] = {0x01, 0x08, 0xFC, 0x0B, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x14, 0x5B, 0xFF, 0x02, 0x01, 0x01};
const uint8_t TCU_HCI_M2_BTL_EEPROM_WRITE_ENABLE_REQ[13] = {0x01, 0x08, 0xFC, 0x09, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x14, 0x83, 0xFF, 0x00};
const uint8_t TCU_HCI_M2_GENERAL_READ_EEPROM_BD_ADDR_REQ[20] = {0x01, 0x08, 0xFC, 0x10, 0x00, 0xA1, 0x00, 0x00, 0x00, 0x14, 0x88, 0xFF, 0x10, 0x06, 0xA0, 0x01, 0x01 ,0x06, 0x02, 0x00};
uint8_t TCU_HCI_WRITE_BD_ADDR_REQ[10] = {0x01, 0x13, 0x10, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TCU_HCI_SET_MODE_REQ[7] = {0x01, 0x08, 0xFC, 0x03, 0x00, 0x99, 0x01};
const uint8_t TCU_MNG_LE_INIT_REQ_ARR[11] = {0x0B, 0x00, 0x00, 0xD1, 0x01, 0x04, 0x00, 0x01, 0x00, 0x01, 0x01};
const uint8_t TCU_LE_GATT_SER_INIT_REQ_ARR[7] = {0x07, 0x00, 0x00, 0xD3, 0x00, 0x00, 0x00};
const uint8_t TCU_MNG_INIT_REQ[16] = {0x10, 0x00, 0x00, 0xE1, 0x01, 0x09, 0x00, 0x04, 0x02, 0x06, 0x75, 0x6E, 0x75, 0x73, 0x65, 0x64};
const uint8_t TCU_LE_GATT_CLI_INIT_REQ[7] = {0x07, 0x00, 0x00, 0xD2, 0x00, 0x00, 0x00 };
uint8_t TCU_MNG_LE_START_ADVERTISE_REQ_ARR[86] = {0x56, 0x00, 0x00, 0xD1, 0x08, 0x4F, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x07, 0x00, 0x03, 0x02, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x06, 0x09, 0x42, 0x49, 0x4F, 0x44, 0x59, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ_ARR[10] = {0x0A, 0x00, 0x00, 0xD3, 0x20, 0x03, 0x00, 0x02, 0x00, 0x18};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ_ARR[13] = {0x0D, 0x00, 0x00, 0xD3, 0x22, 0x06, 0x00, 0x01, 0x00, 0x02, 0x02, 0x00, 0x2A};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ[23] = {0x17, 0x00, 0x00, 0xD3, 0x2C, 0x10, 0x00, 0x10, 0x00, 0x02, 0x00, 0x2A, 0x0D, 0x00, 0x05, 0x00, 0x42, 0x49, 0x4F, 0x44, 0x59, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ2[13] = {0x0D, 0x00, 0x00, 0xD3, 0x22, 0x06, 0x00, 0x01, 0x00, 0x02, 0x02, 0x01, 0x2A};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ_ARR[18] = {0x12, 0x00, 0x00, 0xD3, 0x23, 0x0B, 0x00, 0x20, 0x00, 0x02, 0x01, 0x2A, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ2[10] = {0x0A, 0x00, 0x00, 0xD3, 0x20, 0x03, 0x00, 0x02, 0x0A, 0x18};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ3[13] = {0x0D, 0x00, 0x00, 0xD3, 0x22, 0x06, 0x00, 0x00, 0x01, 0x02, 0x02, 0x29, 0x2A};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ2[25] = {0x19, 0x00, 0x00, 0xD3, 0x23, 0x12, 0x00, 0x10, 0x01, 0x02, 0x29,
			0x2A, 0x09, 0x00, 0x41, 0x6D, 0x69, 0x6E, 0x6F, 0x67, 0x72, 0x61, 0x6D, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ4[13] = {0x0D, 0x00, 0x00, 0xD3, 0x22, 0x06, 0x00, 0x00, 0x01, 0x02, 0x02, 0x24, 0x2A};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ3[27] = {0x1B, 0x00, 0x00, 0xD3, 0x23, 0x14, 0x00, 0x20, 0x01, 0x02, 0x24,
			0x2A, 0x0B, 0x00, 0x4D, 0x6F, 0x64, 0x65, 0x6C, 0x6E, 0x75, 0x6D, 0x62, 0x65, 0x72, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ5[13] = {0x0D, 0x00, 0x00, 0xD3, 0x22, 0x06, 0x00, 0x00, 0x01, 0x02, 0x02, 0x23, 0x2A};
uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4[24] = {0x18, 0x00, 0x00, 0xD3, 0x23, 0x11, 0x00, 0x30, 0x01, 0x02, 0x23, 0x2A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ6[13] = {0x0D, 0x00, 0x00, 0xD3, 0x22, 0x06, 0x00, 0x00, 0x01, 0x02, 0x02, 0x27, 0x2A};
uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ5[27] = {0x1B, 0x00, 0x00, 0xD3, 0x23, 0x14, 0x00, 0x40, 0x01, 0x02, 0x27,
			0x2A, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ3[24] = {0x18, 0x00, 0x00, 0xD3, 0x20, 0x11, 0x00, 0x10, 0x1B, 0xC5, 0xD5,
			0xA5, 0x02, 0x00, 0x03, 0xA9, 0xE3, 0x11, 0x8B, 0xAA, 0xA0, 0xC6, 0x79, 0xE0};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ7[27] = {0x1B, 0x00, 0x00, 0xD3, 0x22, 0x14, 0x00, 0x00, 0x02, 0x28, 0x10,
			0x1B, 0xC5, 0xD5, 0xA5, 0x02, 0x00, 0xEF, 0x9C, 0xE3, 0x11, 0x89, 0xAA, 0xC0, 0x12, 0x83, 0xB3};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ2[189] = {0xBD, 0x00, 0x00, 0xD3, 0x2C, 0xB6, 0x00, 0x10, 0x02, 0x10,
			0x1B, 0xC5, 0xD5, 0xA5, 0x02, 0x00, 0xEF, 0x9C, 0xE3, 0x11, 0x89, 0xAA, 0xC0, 0x12, 0x83, 0xB3, 0x9D, 0x00,
			0x9D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ6[18] = {0x12, 0x00, 0x00, 0xD3, 0x23, 0x0B, 0x00, 0x10, 0x02, 0x02, 0x02,
			0x29, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00};

int COM_initAll(void){
	int res;

	res = COM_initHci();
	if(res)
		return res;

	res = COM_initBLE();
	if(res)
		return res;

	res = COM_initSppOverBLE();
	if(res)
		return res;
    
    res = COM_startToAdvertise();
    if(res)
        return res;

	/* no needed anymore
     * res = COM_initSppBT();
	if(res)
		return STATUS_ERROR;*/

	return STATUS_SUCCESS;
}


int COM_initHci(void){
	int res, i;
	int loop = 0;
    uint8_t* dataPointer;
    trace_info(MODULE_NAME, " *** Hci init... ***\n");
	
	do{
		UART_sendBytes(TCU_HCI_RESET_REQUEST, sizeof(TCU_HCI_RESET_REQUEST));
		res = HCI_receiveResponse(reception_buffer, 7);
		if(res || reception_buffer[4] == 0x00){
			trace_error(MODULE_NAME, "TCU_HCI_RESET_REQUEST response");
			loop++;
			if(loop>2)
				return res;
		}else{
			break;
		}
	}
	while(1);
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_HCI_GET_FW_VERSION_REQ");
	UART_sendBytes(TCU_HCI_GET_FW_VERSION_REQ, sizeof(TCU_HCI_GET_FW_VERSION_REQ));
	res = HCI_receiveResponse(reception_buffer, 33);
	if(res){
		trace_error(MODULE_NAME, "TCU_HCI_GET_FW_VERSION_REQ response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_HCI_M2_BTL_SET_I2C_ENABLE_REQ");
	UART_sendBytes(TCU_HCI_M2_BTL_SET_I2C_ENABLE_REQ, sizeof(TCU_HCI_M2_BTL_SET_I2C_ENABLE_REQ));
	res = HCI_receiveResponse(reception_buffer, 13);
	if(res){
		trace_error(MODULE_NAME, "TCU_HCI_M2_BTL_SET_I2C_ENABLE_REQ response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_HCI_M2_BTL_EEPROM_WRITE_ENABLE_REQ");
	UART_sendBytes(TCU_HCI_M2_BTL_EEPROM_WRITE_ENABLE_REQ, sizeof(TCU_HCI_M2_BTL_EEPROM_WRITE_ENABLE_REQ));
	res = HCI_receiveResponse(reception_buffer, 13);
	if(res){
		trace_error(MODULE_NAME, "TCU_HCI_M2_BTL_EEPROM_WRITE_ENABLE_REQ response");
		return res;
	}
	trace_info("", "\n");

    trace_info(MODULE_NAME, "TCU_HCI_M2_GENERAL_READ_EEPROM_BD_ADDR_REQ");  // cmd to read the bd addr of pan1026a
    UART_sendBytes(TCU_HCI_M2_GENERAL_READ_EEPROM_BD_ADDR_REQ, sizeof(TCU_HCI_M2_GENERAL_READ_EEPROM_BD_ADDR_REQ));
	res = HCI_receiveResponse(reception_buffer, 20);
	if(res){
		trace_error(MODULE_NAME, "TCU_HCI_M2_GENERAL_READ_EEPROM_BD_ADDR_REQ response");
		return res;
	}
    else{
        // get the BD ADDRESS in the response, set it to TCU_HCI_WRITE_BD_ADDR_REQ array
        dataPointer = &BD_ADDRESS_CHAR_DATA_START_INDEX;
        for(i=0; i<BD_ADDRESS_CHAR_DATA_LENGTH; i++)
            dataPointer[5-i] = reception_buffer[14+i];        
    }
	trace_info("", "\n");  
    
	trace_info(MODULE_NAME, "TCU_HCI_WRITE_BD_ADDR_REQ");
	UART_sendBytes(TCU_HCI_WRITE_BD_ADDR_REQ, sizeof(TCU_HCI_WRITE_BD_ADDR_REQ));
	res = HCI_receiveResponse(reception_buffer, 7);
	if(res || reception_buffer[4] != 0x13 || reception_buffer[5] != 0x10){
		trace_error(MODULE_NAME, "TCU_HCI_WRITE_BD_ADDR_REQ response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_HCI_SET_MODE_REQ");
	UART_sendBytes(TCU_HCI_SET_MODE_REQ, sizeof(TCU_HCI_SET_MODE_REQ));
	res = HCI_receiveResponse(reception_buffer, 8);
	if(res){
		trace_error(MODULE_NAME, "TCU_HCI_SET_MODE_REQ response");
		return res;
	}
	trace_info("", "\n");

	// give TC35661 time to come up again
	delay_ms(300);
	
	return STATUS_SUCCESS;
}

int COM_initBLE(void){
	int res;
	trace_info(MODULE_NAME, " *** BLE init... ***\n");

	// BLE app setup
	trace_info(MODULE_NAME, "TCU_MNG_LE_INIT_REQ_ARR");
	UART_sendBytes(TCU_MNG_LE_INIT_REQ_ARR, sizeof(TCU_MNG_LE_INIT_REQ_ARR));
	res = TCU_receiveResponse(reception_buffer, 14);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_MNG_LE_INIT_REQ_ARR response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SER_INIT_REQ_ARR");
	UART_sendBytes(TCU_LE_GATT_SER_INIT_REQ_ARR, sizeof(TCU_LE_GATT_SER_INIT_REQ_ARR));
	res = TCU_receiveResponse(reception_buffer, 8);
	if(res){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SER_INIT_REQ_ARR response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_MNG_INIT_REQ");
	UART_sendBytes(TCU_MNG_INIT_REQ, sizeof(TCU_MNG_INIT_REQ));
	res = TCU_receiveResponse(reception_buffer, 14);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_MNG_INIT_REQ response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_CLI_INIT_REQ");
	UART_sendBytes(TCU_LE_GATT_CLI_INIT_REQ, sizeof(TCU_LE_GATT_CLI_INIT_REQ));
	res = TCU_receiveResponse(reception_buffer, 8);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_CLI_INIT_REQ response");
		return res;
	}
	trace_info("", "\n");

	return STATUS_SUCCESS;
}

int COM_initSppOverBLE(void){
	int res;
	trace_info(MODULE_NAME, " *** Spp over BLE init... ***\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ_ARR");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ_ARR, sizeof(TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ_ARR));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ_ARR response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ_ARR");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ_ARR, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ_ARR));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ_ARR response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ2");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ2, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ2));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ2 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ_ARR");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ_ARR, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ_ARR));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ_ARR response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ2");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ2, sizeof(TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ2));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ2 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ3");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ3, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ3));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ3 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ2");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ2, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ2));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ2 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ4");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ4, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ4));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ4 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ3");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ3, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ3));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ3 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ5");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ5, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ5));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ5 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ4 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ6");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ6, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ6));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ6 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ5");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ5, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ5));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ5 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ3");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ3, sizeof(TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ3));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ3 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ7");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ7, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ7));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ7 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ2");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ2, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ2));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_VAR_ELE_REQ2 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ6");
	UART_sendBytes(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ6, sizeof(TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ6));
	res = TCU_receiveResponse(reception_buffer, 10);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ6 response");
		return res;
	}
	trace_info("", "\n");
    
	return STATUS_SUCCESS;
}

#ifdef NOT_YET_PORTED
int COM_initSppBT(void){
	/*int res;
	trace_info(MODULE_NAME, " *** SPP BT init... ***\n");

	trace_info(MODULE_NAME, "TCU_MNG_INIT_REQ2");
	UART_sendBytes(&TCU_MNG_INIT_REQ2, sizeof(TCU_MNG_INIT_REQ2));
	res = TCU_receiveResponse(reception_buffer, 14);
	if(res || TCU_RESPONSE_BT_INIT_ERROR){
		trace_error(MODULE_NAME, "TCU_MNG_INIT_REQ2 response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_SPP_SETUP_REQ");
	UART_sendBytes(&TCU_SPP_SETUP_REQ, sizeof(TCU_SPP_SETUP_REQ));
	res = TCU_receiveResponse(reception_buffer, 8);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_SPP_SETUP_REQ response");
		return res;
	}
	trace_info("", "\n");

	trace_info(MODULE_NAME, "TCU_MNG_SET_SCAN_REQ");
	UART_sendBytes(&TCU_MNG_SET_SCAN_REQ, sizeof(TCU_MNG_SET_SCAN_REQ));
	res = TCU_receiveResponse(reception_buffer, 8);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_MNG_SET_SCAN_REQ response");
		return res;
	}
	trace_info("", "\n");*/

	return STATUS_SUCCESS;
}
#endif

int COM_startToAdvertise(void){
    
    uint8_t res;
    trace_info(MODULE_NAME, " *** start to advertise ... ***\n");

    trace_info(MODULE_NAME, "TCU_MNG_LE_START_ADVERTISE_REQ_ARR");
	UART_sendBytes(TCU_MNG_LE_START_ADVERTISE_REQ_ARR, sizeof(TCU_MNG_LE_START_ADVERTISE_REQ_ARR));
	res = TCU_receiveResponse(reception_buffer, 8);
	if(res || TCU_STATUS_ERROR){
		trace_error(MODULE_NAME, "TCU_MNG_LE_START_ADVERTISE_REQ_ARR response");
		return res;
	}
	trace_info("", "\n");
    
    return STATUS_SUCCESS;
}
