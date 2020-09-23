/* 
 * File:   
 * Author: Adrien Mallet
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __BUZZER_H
#define	__BUZZER_H

/**
 * 
 * @param frequency : KHz
 * @param nb
 * @param periodOFFX10ms : x *10ms
 * @param periodONX10ms : x *10ms
 */
void BUZZ_loadBuzzer(double frequency, unsigned int nb, unsigned int periodOFFX10ms, unsigned int  periodONX10ms);

void BUZZ_stopBuzzer(void);

void DICond(void);

void EICond(void);

/**
 * 
 * @param frequence : KHz
 * @param Temps : x10ms
 */
void BUZZ_buzzer(double frequency , int periodeX10ms);

void BUZZ_startSuccess(void);

void BUZZ_buzzerTest5DEGRESOK(void);

void BUZZ_buzzerTest5DEGRESPASOK(void);

void BUZZ_successSound(void);

void BUZZ_buzzerBTHS(void);

void BUZZ_errorSound(void);

void BUZZ_buzzerMemoDejaFaite(void);

void BUZZ_buzzerMDKCarteTestHS(void);

void BUZZ_startWaitingConnectionBeeps(void);

void BUZZ_startEchangesInProgressBeeps(void);

/**
 * @brief beeps after measures exchanges failure
 */
void BUZZ_measuresExchangesErrorSound(void);

#endif	/* __BUZZER_H */

