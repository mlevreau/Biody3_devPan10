/*
 * DELAYS_TIME.h
 *
 *  Created on: 18 Oct 2018
 *      Author: Adrien Mallet
 */

#ifndef __DELAYS_TIME_H_
#define __DELAYS_TIME_H_

#include "stdint.h"


#if LINUX
void DELAYS_us(a)   usleep((a));            // for linux 
void DELAYS_ms(a)   usleep((a)*1000);     // for linux 
#endif  //LINUX

#ifdef MPLABX
//#include "pic18.h"
#define _XTAL_FREQ 64000000         // crystal frequency 64MHz
#endif // not MPLABX

//#define DELAYS_us(a)  __delay_us((a))
//#define DELAYS_ms(a)  __delay_ms((a))
/*#else   // not
#define DELAYS_us(a) Delay1KTCYx((a)*10/625)
#define DELAYS_ms(a) Delay1KTCYx(1000*(a)*10/625)
#endif*/


/**
* @brief create a delays of x us
*
* param[in] delay: the delay in uint16_t
*
* @return void
*/
void DELAYS_us(uint16_t delay);

/**
* @brief create a delays of x ms
*
* param[in] delay: the delay in uint16_t
*
* @return void
*/
void DELAYS_ms(uint16_t delay);

#endif /* __DELAYS_TIME_H_ */
