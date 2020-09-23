/* 
 * File:   
 * Author: Adrien Mallet
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __LED_H
#define	__LED_H

#include "stdint.h"


#define GREEN_LED  0
#define RED_LED  1

uint8_t LED_greenLedBlink = 0;      // indicate if green led blinking is on or off
uint16_t LED_greenLedPeriod = 0;    // period of blinking in ms


/**
 * @brief indicate to interruption management that green led has to blink
 * @param period in ms for each state
 */
void LED_startBlinkingGreenLed(uint16_t period);

void LED_stopBlinkingGreenLed(void);

void LED_invertGreenLedState(void);

void LED_enableGreenLed(void);

void LED_enableRedLed(void);

void LED_enableAllLeds(void);

void LED_disableGreenLed(void);

void LED_disableRedLed(void);

void LED_disableAllLeds(void);

void LED_blinkRedLed(void);

void LED_blinkGreenLed(void);

void LED_manageRedLed(unsigned int time);

void LED_manageGreenLed(unsigned int time);

void LED_manageLed(int ledColor, unsigned int time);

#endif	/* __LED_H */

