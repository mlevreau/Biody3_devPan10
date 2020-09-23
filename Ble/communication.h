/*
 * File:
 * Author: Adrien Mallet
 * Comments:
 * Revision history:
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __COMMUNICATION_H
#define	__COMMUNICATION_H

enum communicationState{
    COM_NONE,
    COM_INIT,
	COM_UART_INIT,
	COM_HCI_INIT,
	COM_BLE_INIT,
	COM_SPP_OVER_BLE_INIT,
	COM_SPP_INIT,
	COM_PROFILE_INIT,
	COM_WAITING_BT_CONNECTION,
	COM_BT_CONNECTED,
	COM_BLE_CONNECTED,
	COM_WAITING_EXCHANGE,
	COM_EXCHANGE_IN_PROGRESS,
	COM_EXCHANGE_DONE,
	COM_DISCONNECTED
};

/**
* @brief init all profiles for pan1026a
*
*
* @return 0 : succes
*/
int COM_initAll(void);

/**
* @brief init the Pan1026a with all needed hci commands
*
* param[in] bd_addr[]:
*
* @return 0 : succes
*/
int COM_initHci(void);

/**
* @brief init the ble connection with name etc... until advertisement
*
* @return 0 : succes
*/
int COM_initBLE(void);

/**
* @brief init the spp over ble profile
*
* @return 0 : succes
*/
int COM_initSppOverBLE(void);

/**
* @brief init the spp until advertisement
*
* param[in] bd_addr[]:
*
* @return 0 : succes
*/
int COM_initSppBT(void);

/**
 *@brief start to advertise the profile
 */
int COM_startToAdvertise(void);


#endif	/* __COMMUNICATION_H */
