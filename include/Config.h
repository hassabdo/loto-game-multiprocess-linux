/*************************************************************************
                           Config  -  description
                             -------------------
    début                : 25 Janvier 2025
    copyright            : (C) 2025 par Hassen ABDELJAOUED
    e-mail               : hassen.abdeljaoued@insa-lyon.fr
*************************************************************************/

//---------- Interface du module <Config> (fichier Config.h) -------------
#if ! defined ( CONFIG_H )
#define CONFIG_H

//------------------------------------------------------------------------
// Rôle du module <Config>
//   - Fichier de configuration de l'application multitâche
//   - Pas de réalisation rattachée à cette interface
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

// <MIN_BOULE> et <MAX_BOULE> définissent les bornes de l'intervalle
// pour le tirage des nombres
// => tous les tirages appartiennent à [ <MIN_BOULE> , <MAX_BOULE> ]
const unsigned int MIN_BOULE =  1;
const unsigned int MAX_BOULE = 49;

// Taille du tableau utilisé pour mémoriser les différents tirages.
// La taille du tableau est construit à partir de MIN_BOULE et MAX_BOULE
const unsigned int MAX = MAX_BOULE - MIN_BOULE + 1;

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

#endif // CONFIG_H