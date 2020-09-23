/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __BLE_TCU_COMMANDS_H
#define	__BLE_TCU_COMMANDS_H

//*************** SERVICE ID **************************

/*! TCU service id enumeration */
enum tcu_service_ids {
  TCU_LE_SERVICE_MNG = 0xD1,      /*!< low-energy management service id */
  TCU_LE_SERVICE_GATT_CLI = 0xD2, /*!< GATT client service id */
  TCU_LE_SERVICE_GATT_SRV = 0xD3, /*!< GATT server service id */
  TCU_LE_SERVICE_SMP_MAS = 0xD4,  /*!<  SMP master service id */
  TCU_LE_SERVICE_SMP_SLV = 0xD5,  /*!< SMP slave service id */
  TCU_BT_SERVICE_VEN = 0xEF,      /*!< vendor service id */
  TCU_BT_SERVICE_MNG = 0xE1,      /*!< classic management service id */
  TCU_BT_SERVICE_SPP = 0xE5       /*!< SPP profile service id */
};

//* TCU_LE_MNG_OPCODE //
#define TCU_MNG_LE_INIT_REQ 0x01
#define TCU_MNG_LE_SET_RAND_ADDRESS_REQ 0x04
#define TCU_MNG_LE_READ_WHITELIST_SIZE_REQ 0x05
#define TCU_MNG_LE_ADD_DEVICE_TO_WHITELIST_REQ 0x06
#define TCU_MNG_LE_DEL_WHITELIST_REQ 0x07
#define TCU_MNG_LE_START_ADVERTISE_REQ 0x08
#define TCU_MNG_LE_DISABLE_ADVERTISE_REQ 0x09
#define TCU_MNG_LE_SET_SCAN_ENABLE_REQ 0x0A
#define TCU_MNG_LE_SET_SCAN_DISABLE_REQ 0x0B
#define TCU_MNG_LE_SET_ADVERTISE_DATA_REQ 0x1D
#define TCU_MNG_LE_CREATE_CONNECTION_REQ 0x0C
#define TCU_MNG_LE_CREATE_CONNECTION_CANCEL_REQ 0x0D
#define TCU_MNG_LE_CONNECTION_UPDATE_REQ 0x0E
#define TCU_MNG_LE_GEN_NON_RESOLVABLE_BDADDR_REQ 0x21
#define TCU_MNG_LE_GEN_NON_RESOLVABLE_BDADDR_RESP 0xA1
#define TCU_MNG_LE_DISCONNECT_REQ 0x13
#define TCU_MNG_LE_CON_UPDATE_ACCEPT_REQ 0x16
#define TCU_MNG_LE_GEN_RESOLVABLE_BDADDR_REQ 0x17
#define TCU_MNG_LE_RESOLVE_BDADDR_REQ 0x18
#define TCU_MNG_LE_SET_IR_VALUE_REQ 0x1B
#define TCU_MNG_LE_REM_CON_PARAM_ACCEPT_REQ 0x1E
#define TCU_MNG_LE_CONNECTION_COMPLETE_EVENT 0x4C       // 1st connection
#define TCU_MNG_LE_CONNECTION_UPDATE_EVENT 0x4E
#define TCU_MNG_LE_L2CAP_CONNECTION_UPDATE_RESP_EVENT 0x50
#define TCU_MNG_LE_UPDATE_CONN_REQ_EVENT 0x52
#define TCU_MNG_LE_REM_CON_PARAM_REQ_EVENT 0x5E
#define TCU_MNG_LE_READ_WHITELIST_SIZE_RESP 0x85
#define TCU_MNG_LE_ADD_DEVICE_TO_WHITELIST_RESP 0x86
#define TCU_MNG_LE_DEL_WHITELIST_RESP 0x87
#define TCU_MNG_LE_DISCONNECT_EVENT 0x93
#define TCU_MNG_LE_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_REQ 0x1F
#define TCU_MNG_LE_READ_AUTHENTICATED_PAYLOAD_TIMEOUT_RESP 0x9F
#define TCU_MNG_LE_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_REQ 0x20
#define TCU_MNG_LE_WRITE_AUTHENTICATED_PAYLOAD_TIMEOUT_RESP 0xA0
#define TCU_MNG_LE_GET_LE_STATUS_REQ 0x22
#define TCU_MNG_LE_GET_LE_STATUS_RESP 0xA2
#define TCU_MNG_LE_READ_CHANNEL_MAP_REQ 0x11
#define TCU_MNG_LE_READ_CHANNEL_MAP_RESP 0x91
#define TCU_MNG_LE_SET_HOST_CHANNEL_CLASSIFICATION_REQ 0x10
#define TCU_MNG_LE_SET_HOST_CHANNEL_CLASSIFICATION_RESP 0x90
#define TCU_MNG_LE_READ_TX_POW_LEVEL_REQ 0x14
#define TCU_MNG_LE_READ_TX_POW_LEVEL_RESP 0x94
#define TCU_MNG_LE_READ_RSSI_REQ 0x15
#define TCU_MNG_LE_READ_RSSI_RESP 0x95
#define TCU_MNG_LE_SET_DATA_LENGTH_REQ 0x26
#define TCU_MNG_LE_SET_DATA_LENGTH_RESP 0xA6
#define TCU_MNG_LE_SET_DATA_LENGTH_CHANGE_EVENT 0x66
#define TCU_MNG_LE_GENERATE_OOB_DATA_REQ 0x25


/* GATT Server Service Opcode */
#define TCU_LE_GATT_SER_INIT_REQ 0x00
#define TCU_LE_GATT_SER_EXG_MTU_ACCEPT_REQ 0x01
#define TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_REQ 0x02
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_REQ 0x03
#define TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_REQ 0x04
#define TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_REQ 0x05
#define TCU_LE_GATT_SER_CHAR_VAL_INDICATION_REQ 0x06
#define TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_REQ 0x08
#define TCU_LE_GATT_SER_READ_LONG_CHAR_DESP_ACCEPT_REQ 0x0E
#define TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_REQ 0x0A
#define TCU_LE_GATT_SER_READ_LONG_CHAR_VAL_ACCEPT_REQ 0x0D
#define TCU_LE_GATT_SER_CHAR_VAL_INDICATION_EVENT 0x46
#define TCU_LE_GATT_SER_CHAR_VAL_NOTIFICATION_EVENT 0x45
#define TCU_LE_GATT_SER_EXG_MTU_ACCEPT_RESP 0x81
#define TCU_LE_GATT_SER_READ_CHAR_VAL_ACCEPT_RESP 0x82
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_ACCEPT_RESP 0x83
#define TCU_LE_GATT_SER_WRITE_CHAR_DESP_ACCEPT_RESP 0x84
#define TCU_LE_GATT_SER_READ_CHAR_DESP_ACCEPT_RESP 0x88
#define TCU_LE_GATT_SER_READ_LONG_CHAR_DESP_ACCEPT_RESP 0x8E
#define TCU_LE_GATT_SER_READ_MULTIPLE_ACCEPT_RESP 0x8A
#define TCU_LE_GATT_SER_READ_LONG_CHAR_VAL_ACCEPT_RESP 0x8D
#define TCU_LE_GATT_SER_EXG_MTU_EVENT 0xC1
#define TCU_LE_GATT_SER_READ_CHAR_VAL_EVENT 0xC2
#define TCU_LE_GATT_SER_WRITE_CHAR_VAL_EVENT 0xC3
#define TCU_LE_GATT_SER_WRITE_CHAR_DESP_EVENT 0xC4
#define TCU_LE_GATT_SER_READ_CHAR_DESP_EVENT 0xC8
#define TCU_LE_GATT_SER_READ_LONG_CHAR_DESP_EVENT 0xCE
#define TCU_LE_GATT_SER_WRITE_WITHOUT_RESPONSE_CMD_EVENT 0xC9
#define TCU_LE_GATT_SER_READ_MULTIPLE_EVENT 0xCA
#define TCU_LE_GATT_SER_READ_LONG_CHAR_VAL_EVENT 0xCD


/* GATT Server SDB Opcodes */
#define TCU_LE_GATT_SDB_ADD_PRIM_SVC_REQ 0x20
#define TCU_LE_GATT_SDB_ADD_CHAR_DECL_REQ 0x22
#define TCU_LE_GATT_SDB_ADD_CHAR_ELE_REQ 0x23
#define TCU_LE_GATT_SDB_ADD_VAR_CHAR_ELE_REQ 0x2C
#define TCU_LE_GATT_SDB_UPD_CHAR_ELE_REQ 0x25
#define TCU_LE_GATT_SDB_ADD_ATTRIBUTE_REQ 0x27
#define TCU_LE_GATT_SDB_UPD_CHAR_ELE_RESP 0xA5


/*! Definition of TCU_LE_ACCEPT */
#define TCU_LE_ACCEPT 0xF1
/*! Definition of TCU_LE_NOT_ACCEPT */
#define TCU_LE_NOT_ACCEPT 0xF2
/*! Definition of TCU_LE_FATAL_ERROR */
#define TCU_LE_FATAL_ERROR 0xFE



#endif	/* __BLE_TCU_COMMANDS_H  */

