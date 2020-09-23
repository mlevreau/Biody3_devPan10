/*
 * VERSION.h
 *
 *  Created on: 11 Nov 2018
 *      Author: Adrien Mallet
 */

 
#ifndef __VERSION_H
#define	__VERSION_H

// build info
//#include "REL_V69_build.h" par Marilys Levreau

// ************************* INFORMATIONS ************************
#define DEV_TEST 0

#define FW_VERSION 69
#define DEVICE_MODEL_NAME_SIZE 5
uint8_t DEVICE_MODEL_NAME[] = {'B', 'I', 'O', 'D', 'Y'};

// master key
const uint8_t DEFAULT_MASTER_CRYPT_KEY[4] = {0x3F, 0x28, 0x4C, 0xB7};

// info of biody type, used to recognized a specific type of Biody before connection
uint8_t BIODY_SYSTEM_TYPE = 0x01;

#endif	/* __VERSION_H */



























