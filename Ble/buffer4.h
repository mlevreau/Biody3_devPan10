//$f------------------------------------------------------------------------
//       (c) MEDELEK 2013.                   Tous Droits Reserves       
//
// Projet   : 
//
// Fichier  : buffer4.H
//
// Version  : 1.00
//
// Contenu  : Prototypes du module MESURE
// 
// Createur : Bruno Beltramini
//
// Date     : 1/1/06  
//
// Modifs   :
//
//    Auteur    |  Date  |Version|Nature
//--------------|--------|-------|------------------------------------------
// B. Beltramini|19/2/13 | 01.00 |Creation
//--------------------------------------------------------------------------
//
// Info     :
//
//$f------------------------------------------------------------------------

#ifndef BUFFER4_H

#define BUFFER4_H
EXTPUBL int TableMesureReglageZmaxPHIVOZmin[DERNIEREFREQUENCE+1][4]; // Zmax PHI VO et ZMin x 11 Freq pour reglage
EXTPUBL int TableMesureGrandeursZPHI1[DERNIEREFREQUENCE+1][2]; // Z et PHI calculés en Ohm et ° 11 Freq
EXTPUBL int TableMesureGrandeursZPHI2[DERNIEREFREQUENCE+1][2]; // Z et PHI calculés en Ohm et ° 11 Freq
EXTPUBL int TableMesureZPHIVO[DERNIEREFREQUENCE+1][3]; //Z PHI et VO mesurés

EXTPUBL union
	{
	struct struct2Char  {
    unsigned char Low:8;
    unsigned char High:8; 
						}Octet;
	unsigned int Entier;
	}Structure16Bits,PeriodeBuzzer;

EXTPUBL union {
  struct {
    unsigned B0:1;
    unsigned B1:1;
    unsigned B2:1;
    unsigned B3:1;
    unsigned B4:1;
    unsigned B5:1;
    unsigned B6:1;
    unsigned B7:1;
  };
 
 unsigned char CAR;
 } DataPort1,DataPort2,Data8;	 // V0.9	

EXTPUBL unsigned char k;
EXTPUBL unsigned char VoieOk;
#endif


