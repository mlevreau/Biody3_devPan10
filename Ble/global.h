 //$f------------------------------------------------------------------------
//       (c) MEDELEK 2008.                   Tous Droits Reserves       
//
// Projet   : BIOLIGHT
//
// Fichier  : GLOBAL.H
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include "stdint.h"

//not used for marilys test
//unsigned char TableFrequence[11] = {5,10,20,40,45,50,55,60,100,150,200};


//____________________________________________

unsigned char NiveauVBAT; //V0.8
unsigned char RepriseMesure; //V0.8
unsigned char ReglageParTesteur;
uint16_t Valim2;
unsigned char FinInitBT ; 
unsigned char Role;
unsigned char Retour;
unsigned char NBupdate;


 //pour mesures segmentaires : enum {SEGMENTBD = 0,SEGMENTBG,SEGMENTJD,SEGMENTJG,SEGMENTDIAGD,SEGMENTDIAGG,SEGMENTTRONC};
int16_t TableMesureGrandeursSegment3FreqZ[DERNIERSEGMENT+1][3]; // Segment/(Z5 Z50 Z200kHZ)  :calculés en Ohm  pour 7 segments pour 3 frequences
/*
avec :
#define BRASetJAMBEDROITetTRONC 0 // M1 total voie 1
#define BRASetJAMBEGAUCHEetTRONC 0xFF // M2 total voie 2
#define JAMBEDROITEetTRONC 0x88 // M3
#define JAMBEGAUCHEetTRONC 0x77// M4
#define BRASDROITetTRONC 0x44 // M5 
#define BRASGAUCHEetTRONC 0xBB // M6
#define TRONC 0xCC
#define DIAGONALEDROITE 0x55 // M7
#define DIAGONALEGAUCHE 0xAA // M8
enum {SEGMENTBD = 0,SEGMENTBG,SEGMENTJD,SEGMENTJG,SEGMENTTRONC,SEGMENTDIAGD,SEGMENTDIAGG,PASDESEGMENT};
#define DERNIERSEGMENT SEGMENTDIAGG
enum {F5K= 0,F50K,F200K};

 on a les mesures suivantes :

SEGMENTBD = BRASetJAMBEDROITetTRONC(M1) - JAMBEDROITEetTRONC(M3)
SEGMENTBG = BRASetJAMBEGAUCHEetTRONC(M2) - JAMBEGAUCHEetTRONC(M4)
SEGMENTJD = BRASetJAMBEDROITetTRONC(M1) - BRASDROITetTRONC(M5)
SEGMENTJG = BRASetJAMBEGAUCHEetTRONC(M2) - BRASGAUCHEetTRONC(M6)
SEGMENTDIAGD = DIAGONALEDROITE (M7) + PHASE A 50kHz
SEGMENTDIAGG =  DIAGONALEGAUCHE(M8) + PHASE A 50kHz
SEGMENTTRONC = (M1 + M2 - SEGMENTBD + SEGMENTBG + SEGMENTJD+ SEGMENTJG )/2

*/


#endif


