 //$f------------------------------------------------------------------------
//       (c) MEDELEK 2008.                   Tous Droits Reserves       
//
// Projet   : BIOLIGHT
//
// Fichier  : CONST.H
//
// Version  : 1.00
//
// Contenu  : Module principal de l'application
// 
// Createur : Bruno Beltramini
//
// Date     : 21/10/08  
//
//
//    Auteur    |  Date  |Version|Nature
//--------------|--------|-------|------------------------------------------
// B. Beltramini|21/10/08| 01.00 |Creation

/*
 
//_______________________________________________________________________________________________


//$f------------------------------------------------------------------------
*/

#ifndef CONST_H
#define CONST_H

#define VRAI 1
#define FAUX 0
#define EI 1
#define DI 0
#define RET_OK 0
#define RET_KO 1
#ifndef NULL
#define NULL 0
#endif
#define ETX 0x03


// |---------|------------|-----------------------|
// |  Type   |   Taille   |        Echelle        |
// |---------|------------|-----------------------|
// | BOOLEAN |	 1 bits	  |          0->1		  |
// | BYTE    |   8 bits   |          0->255       |
// | CHAR	 |   8 bits   |       -128->127       |
// | WORD	 |  16 bits   |          0->65535     |
// | INT	 |	16 bits   |     -32768->32767     |
// | DWORD	 |  32 bits   |		     0->4294967295|
// | LONG	 |	32 bits   |-2147483648->2147483647|
// | FLOAT   |  32 bits   |   1.18e-38->3.4e38    |
// | DOUBLE  |  32 bits   |   1.18e-38->3.4e38    |
// |---------|------------|-----------------------|
typedef unsigned char      BOOLEAN;
typedef unsigned char      BYTE;
typedef char               CHAR;
typedef unsigned short int WORD;
typedef short int          INT;
typedef unsigned long      DWORD;
typedef float              FLOAT;
typedef double             DOUBLE;




//______________ Constantes a ajouter ds const.H

#define T3_FDV304P 1 //V0.8

#define BAT 0
#define PH2 1
#define VZ 2
#define V0 3
#define VGENE 4

//#define VGENEMAX  380 // 1024*1.5/4,095 3V pic avec pont diviseur par 2 
//#define VGENEMAX  550 // 1024*2.2/4,095 4,5V pic avec pont diviseur par 2  max = 625 pour 5V 
#define VGENEMAX  563 // V0.7  1024*2.25/4,095 4,5V pic avec pont diviseur par 2  max = 625 pour 5V 
//V0.22#define VGENEMIN  20 // court circuit ? = 20*4mV *2 = 160mV 1,1mA @ 50 Ohm =
#define VGENEMIN  2 // court circuit ? = 2*4mV *2 = 16mV 1,1mA @ 50 Ohm =

//#define ChargeON (PORTDbits.RD5 = 1)
//#define AnaON (PORTDbits.RD0 = 1)
//#define ChargeOFF (PORTDbits.RD5 = 0)
//#define AnaOFF (PORTDbits.RD0 = 0)

#define PWRBTOFF 0
#define PWRBTON (PORTAbits.RA7 = 1)

#define ALimON (PORTBbits.RB5 = 1)
#define AlimOFF (PORTBbits.RB5 = 0) 
#define BUZON (PORTCbits.RC2 = 1)
#define BUZOFF (PORTCbits.RC2 = 0)
#define PinTEST PORTAbits.RA6 // attention = CSPRO !
//#define SELEXT_ON (PORTAbits.RA7 = 0)
//#define SELEXT_OFF (PORTAbits.RA7 = 1)
#define CSPRO_OFF (PORTAbits.RA6 = 0)//V0.3
#define CSPRO_ON (PORTAbits.RA6 = 1)//V0.3
#define CLKTEST_0 (PORTAbits.RA4 = 0)//V0.9
#define TESTCLKTEST (PORTAbits.RA4 == 1)//V0.65
#define CLKTEST_1 (PORTAbits.RA4 = 1)//V0.9
#define MOSITEST (PORTCbits.RC4)//V0.9
#define RCLKTEST_0 (PORTAbits.RA6 = 0)//V0.9
#define RCLKTEST_1 (PORTAbits.RA6 = 1)//V0.9
#define LEDENCOURS RCLKTEST_1 
// V0.9 #define CSTEST_OFF (PORTAbits.RA4 = 0)//V0.3
//V0.9 #define CSTEST_ON (PORTAbits.RA4 = 1)//V0.3
//#define TESTCARTE (PORTAbits.RA4 == 0)//V0.9
#define CSEXT_ON 	(PORTA = (PORTA | 0x50))
#define CSEXT_OFF (PORTA = (PORTA & 0xAF))

// V0.9 #define SPIIN (PORTCbits.RC4)//V0.3
#define TIMEOUTADCONV  2 // x 10ms  mini : 28 TAD pour conversion 
#define SELPOT_ON (PORTBbits.RB0 = 0)
#define SELPOT_OFF (PORTBbits.RB0 = 1)

#define GAINVBAT 12 //V0.8 4,99K -> 3*4095/1024   3(pont diviseur)*4095mV(Vref)/1024(pts conv max)
#define TEMPOMESURE 10 // x 10ms 
// pour prendre en compte la chute de tension dans mosfet pour mesure tension pile 
/* V0.62#if (MEDICAL)
#define RESET_CON PORTBbits.RB1
#else
*/
#define CSUSB (PORTBbits.RB1 = 1) //V0.24
#define CSBT (PORTBbits.RB1 = 0)  //V0.24
//V0.62 #endif
// V0.9 #define CON_OK PORTBbits.RB2
//#define CTSBT PORTBbits.RB3 remplacé par ST2 , CTS tjs valide !
#define RTSBT PORTBbits.RB4
#define LECBP (PORTBbits.RB3)
#define CHARGE (PORTCbits.RC0)
#define BUZ (PORTCbits.RC2)

#define TIMEOUTCDEBT 100
#define CARACMAITRE DataPort1.CAR

#define TAILLEBUFFERRECEPTION 80
#define TAILLEBUFFEREMISSION 255 



enum  {TSU=0,US2A,KAV,RF,DEPRESSO,BIOLIGHT,IBU232=10,BT,USB};

#define NBMESURE 32 // doit etre paire !

// V0.4 #define TEMPOPC 3000 // x 10ms 
// V0.24#define TIMEOUTPC 3000
#define TIMEOUTPC 5000
#define TIMEOUTPCAPPAIRAGE 6000
#define TEMPOPCFIN 500 // x 10ms 
enum {BIO= 0,SEG,MED};
#define BRASetJAMBEDROITetTRONC 0 // M1 total voie 1
#define BRASetJAMBEGAUCHEetTRONC 0xFF // M2 total voie 2
#define JAMBEDROITEetTRONC 0x88 // M3
#define JAMBEGAUCHEetTRONC 0x77// M4
#define BRASDROITetTRONC 0x44 // M5 
#define BRASGAUCHEetTRONC 0xBB // M6
#define TRONC 0xCC
#define DIAGONALEDROITE 0x55 // M7
#define DIAGONALEGAUCHE 0xAA // M8


// pour TableMesureReglageZmaxZminVO [Mxk,MxR]
enum {M5k= 0,M10k,M20k,M40k,M45k,M50k,M55k,M60k,M100k,M150k,M200k}; // NE PAS CHANGER !!!!
#define DERNIEREFREQUENCE M200k

int TableMesureGrandeursZPHI1[DERNIEREFREQUENCE+1][2];
unsigned char NbCarReception1;
unsigned int volatile TimeOUT;

enum {MZMAXR= 0,MPHIZEROR,MVOR,MZMINR};
enum {SEGMENTBD = 0,SEGMENTBG,SEGMENTJD,SEGMENTJG,SEGMENTTRONC,SEGMENTDIAGD,SEGMENTDIAGG,PASDESEGMENT};
#define DERNIERSEGMENT SEGMENTDIAGG
enum {F5K= 0,F50K,F200K};

// pour TableMesureZPHIVO[11][3]
enum {MZ= 0,MPHI,MVO};
#define ADRESSEPARAM 0
#define ADRESSEMEMOMESURE (8*M200k+16) //96 avec M200k = 10
#define ADRESSESEGMENT ADRESSEMEMOMESURE+10+8*(unsigned int)(M200k) //
#define ADRESSEPHASEDIAGO (ADRESSESEGMENT+6*(unsigned int)(DERNIERSEGMENT+1)+2)

// V0.14 #define ADRESSEFINMEMOEEPROM (ADRESSEPHASEDIAGO+6)
#define ADRESSEFORCEMESURE5DEGRES (ADRESSEPHASEDIAGO+6)

// Data stored in eeprom
#define DEVICE_ID_STORED_MEM_ADDRESS (ADRESSEFORCEMESURE5DEGRES+2) 
#define DEVICE_ID_MEM_SIZE 6
#define EXCHANGE_KEY_MEM_ADDRESS (DEVICE_ID_STORED_MEM_ADDRESS + DEVICE_ID_MEM_SIZE)        // insert here more Ce 
#define EXCHANGE_KEY_MEM_SIZE 4
#define MASTER_KEY_MEM_ADDRESS (EXCHANGE_KEY_MEM_ADDRESS + EXCHANGE_KEY_MEM_SIZE)
#define MASTER_KEY_MEM_SIZE 4
#define SERIAL_NUMBER_MEM_ADDRESS (MASTER_KEY_MEM_ADDRESS + MASTER_KEY_MEM_SIZE)
#define SERIAL_NUMBER_MEM_SIZE 10

#define ADRESSEFINMEMOEEPROM (DEVICE_ID_STORED_MEM_ADDRESS + DEVICE_ID_MEM_SIZE + EXCHANGE_KEY_MEM_SIZE + SERIAL_NUMBER_MEM_SIZE)

#define MAX_DATA_EEPROM_ADDRESS 0x3FF

#define REGLAGEPARTESTEUR ADRESSEFINMEMOEEPROM+2 //V0.67

#define FOR_K_DE_M5K_A_M200K for(k = M5k;k <(M200k+1);k++)
#define FOR_I_DE_F5K_A_F200K for( i= F5K;i<(F200K+1);i++) //V0.5 oubli parentheses ...

#define CR
#define LF 0x0A
#define MAXCAR 50

#define COEFF_TENSION_ANA_TEST
#define LEDVERTEON (PORTAbits.RA4 = 1)
#define LEDVERTEOFF (PORTAbits.RA4 = 0)
#define LEDROUGEON (PORTCbits.RC4 = 1)
#define LEDROUGEOFF (PORTCbits.RC4 = 0)
#define RESETPAN1026 (PORTAbits.RA6 = 0)
#define SETPAN1026 (PORTAbits.RA6 = 1)

#endif
