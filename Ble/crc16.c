
#include "stdint.h"
#include "crc16.h"



/*uint16_t CRC16_calculateCrc16(const unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0x1021;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}*/

uint16_t CRC16_calculateCrc16Xmodem(uint8_t *bytes, uint8_t length)
{
    uint16_t generator = 0x1021; 	/* divisor is 16bit */
    uint16_t crc = 0; 				/* CRC value is 16bit */

    for(int y=0; y< length; y++)
    {
        crc ^= ((uint16_t)(bytes[y] << 8)); /* move byte into MSB of 16bit CRC */

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x8000) != 0) 	/* test for MSB = bit 15 */
            {
                crc = ((uint16_t)((crc << 1) ^ generator));
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}
