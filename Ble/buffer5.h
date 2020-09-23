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

#ifndef BUFFER5_H
#define BUFFER5_H

#include "stdint.h"

EXTPUBL int iResult;
EXTPUBL unsigned int volatile Timer10msBUZ;
EXTPUBL unsigned int volatile Timer10msDelai;
EXTPUBL unsigned int volatile TimeOUT;
EXTPUBL unsigned int volatile TimeOUT2;
EXTPUBL unsigned char volatile Tempo10ms;

uint8_t timePassed = 0x00;


EXTPUBL  int MesureVBAT;

EXTPUBL int MesurePhaseZeroAZmaxReglageCalcul50kHz;
EXTPUBL int MesurePhaseZeroAZminReglage50kHz;
EXTPUBL int MesurePhase5DegAZmaxReglage50kHz;
EXTPUBL int MesurePhase10DegAZmaxReglage50kHz;
EXTPUBL int MesurePhase50kHzDiagDroit;
EXTPUBL int MesurePhase50kHzDiagGauche;

EXTPUBL int MesureGENE;

// valeur de réglage pot pour les différentes fréquences

EXTPUBL long MesurePH2;
EXTPUBL long MesureV0;
EXTPUBL long MesureVZ;
EXTPUBL long MesureTampon;
//EXTPUBL char Chaine[20];
EXTPUBL unsigned char Erreur;
EXTPUBL unsigned char Erreur2;
EXTPUBL unsigned char	volatile Tempo10msADConv;

//EXTPUBL unsigned char NoDevice;
EXTPUBL unsigned char TempsLECBP;
EXTPUBL unsigned char LECBPFiltre;
EXTPUBL unsigned char MemoLECBP;
EXTPUBL unsigned char ErreurPh2;
EXTPUBL unsigned char ErreurVO;
EXTPUBL unsigned char ErreurVZ;
EXTPUBL unsigned char ErreurGR;

EXTPUBL unsigned char Tamp;
EXTPUBL unsigned int TempoDialPC; //V0.2
//V0.24 EXTPUBL unsigned int TempoReglage; //V0.2
EXTPUBL uint16_t TempoLEDVerte; //V0.24

EXTPUBL unsigned int measureInMemory;
EXTPUBL unsigned int IntTampon,IntTampon2;
//EXTPUBL int EntierTampon;

EXTPUBL unsigned char TypeCarte;
EXTPUBL unsigned char TypeCarteMemorise;

EXTPUBL int TableMesureGrandeursZPHI[DERNIEREFREQUENCE+1][2]; // Z et PHI calculés en Ohm et ° 11 Freq
EXTPUBL int MesureGENEBUF; //V0.24
EXTPUBL uint16_t TempoLEDRouge; 

#endif


