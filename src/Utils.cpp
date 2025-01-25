/*************************************************************************
                           Utils  -  description
                             -------------------
    début                : 25 Janvier 2025
    copyright            : (C) 2025 par Hassen ABDELJAOUED
    e-mail               : hassen.abdeljaoued@insa-lyon.fr
*************************************************************************/

//---------- Réalisation du module <Utils> (fichier Utils.cpp) ---------------

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <signal.h>
//------------------------------------------------------ Include personnel
#include "Utils.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void initSignal ( int noSignal, void (*handler) (int) )
// mode d'emploi :
//    <noSignal> : numéro du signal dont le comportement va changer
//    <handler>  : nouveau handler à rattacher au signal <noSignal>
//    - modification du handler attaché au signal <noSignal>
//    - handler associé : handler
//    - pas de reprise des appels systèmes interrompus
{
    struct sigaction action;

    action.sa_handler = handler;
    sigemptyset ( &action.sa_mask );
    action.sa_flags = 0;

    sigaction ( noSignal, &action, NULL );

} //----- fin de initSignal