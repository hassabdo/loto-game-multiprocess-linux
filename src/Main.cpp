/*************************************************************************
                           Main  -  description
                             -------------------
    début                : 25 Janvier 2025
    copyright            : (C) 2025 par Hassen ABDELJAOUED
    e-mail               : hassen.abdeljaoued@insa-lyon.fr
*************************************************************************/

//---------- Réalisation du module <Main> (fichier Main.cpp) -------------

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <iostream>
using namespace std;

#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//------------------------------------------------------ Include personnel
#include "Config.h"
#include "Utils.h"
#include "Loto.h"
#include "Tirage.h"
#include "Main.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static bool  isAuthorized = true;
static pid_t pidLoto;
static pid_t pidTirage;


//------------------------------------------------------ Fonctions privées
static void toggleTirage ( int noSignal )
// mode d'emploi :
//    <noSignal> : correspond au numéro de signal source du déclenchement
//    - Ce paramètre n'est pas utilisé ici (une seule possibilité)
//    - Ce handler bascule l'état du processus <Tirage>
//        a. s'il est "ON", il passe en "OFF"
//        b. s'il est "OFF", il passe en "ON"
//    - Ce handler utilise la suspension / réactivation d'un processus
//
{
#if defined MAP
        cout << ">>> " << ( isAuthorized ? "Suspension" : "Réactivation" )
             << " du processus <Tirage>..." << endl;
#endif
    kill ( pidTirage, isAuthorized ? SIGSTOP : SIGCONT );
    isAuthorized = ( isAuthorized ? false : true );
    
} //----- fin de toggleTirage


static void finApplication ( int noSignal )
// mode d'emploi :
//    <noSignal> : correspond au numéro de signal source du déclenchement
//    - Ce handler traite la demande de fin de l'application multitâche
{
    if ( noSignal == SIGINT )
    {
        // Modification du signal <SIGINT> inutile => automatiquement bloqué

        // Masquage de <SIGUSR2> pour éviter une demande de fin de l'application
        // (suite à un doublon) alors qu'il y en a déjà une en cours
        // (utilisation de Ctrl-C)
        initSignal ( SIGUSR2,  SIG_IGN );

        // Le signal <SIGINT> a été "broadcasté" à tous les processus fils
        // => la demande de fin est déjà en cours et il n'y a plus qu'à
        //    attendre la mort de chacun des processus fils

        // Il peut y avoir un souci avec la suspension du processus <Tirage>...
        // => le processus <Tirage> doit être actif pour répondre au signal
        //    <SIGINT> de terminaison
        if ( ! isAuthorized )
        {
            kill ( pidTirage, SIGCONT );
        }

        // Attente de la fin du processus <Tirage>
        waitpid ( pidTirage, NULL, 0 );

        // Attente de la fin du processus <Loto>
        waitpid ( pidLoto, NULL, 0 );
#if defined MAP
        cout << ">>> Fin du processus <Main> suite à un Ctrl-C..." << endl;
#endif
    }
    else
    {
        // Modification du signal <SIGUSR2> inutile => automatiquement bloqué

        // Masquage de <SIGINT> pour éviter une demande de fin de l'application
        // alors qu'il y en a déjà une en cours (doublon au tirage)
        initSignal ( SIGINT,  SIG_IGN );

        // Le processus <Loto> est déjà mort
        // => il a détecté le doublon, l'a signalé et s'est arrêté

        // Le processus <Tirage> s'arrêtera tout seul puisqu'il n'y a plus
        // de consommateur sur le canal de communication nommé
        // => il suffit d'attendre sa mort...
        // NB: ce n'est pas la fin proposée en séance
        //     le signal SIGPIPE n'est pas utilisé dans la solution du cours

        // Il peut y avoir un souci avec la suspension du processus <Tirage>...
        // => le processus <Tirage> doit être actif pour détecter l'absence
        //    de lecteur
        if ( ! isAuthorized )
        {
            kill ( pidTirage, SIGCONT );
        }
        waitpid ( pidTirage, NULL, 0 );
        waitpid ( pidLoto, NULL, 0 );
#if defined MAP
        cout << ">>> Fin du processus <Main> suite à un doublon..." << endl;
#endif
    }
    exit ( 0 );

} //----- fin de finApplication


//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
int main ( void )
// Algorithme :
//
{
#if defined MAP
    cout << ">>> Début du processus <Main>..." << endl;
#endif

    // Ignorance des signaux de l'application dans sa phase de mise en place
    initSignal ( SIGINT,  SIG_IGN );
    initSignal ( SIGQUIT, SIG_IGN );
    initSignal ( SIGPIPE, SIG_IGN );
    initSignal ( SIGUSR1, SIG_IGN );
    initSignal ( SIGUSR2, SIG_IGN );

    // Création du canal de communication anonyme entre les processus
    // <Tirage> et <Loto>
    // pipe (  );

    if ( ( pidLoto = fork ( ) ) == 0 )
    {   // Lancement du processus <Loto>
        // (affichage périodiques des nombres)
        // Loto ( );
    }
    else
    {
        if ( ( pidTirage = fork ( ) ) == 0 )
        {   // Lancement du processus <Tirage>
            // (tirage périodique d'un nombre)
            // Tirage ( pidLoto );
        }
        else
        {
            // Armement du signal <SIGQUIT> pour intercepter les demandes de
            // changement d'état du processus <Tirage>
            //   a. Passage de isAuthorized à interdiction de tirage de boules
            //   b. Passage de interdiction à isAuthorized de tirage de boules
            initSignal ( SIGQUIT, toggleTirage );

            // Armement du signal <SIGUSR2> pour intercepter une demande de fin 
            // de la part du processus <Loto>
            // 1. Fin déclenchée par le processus <Loto> suite à l'apparition
            //    d'un tirage en double
            initSignal ( SIGUSR2, finApplication );

            // Armement du signal <SIGINT> pour intercepter la fin de l'application
            // 2. Fin déclenchée au clavier par l'utilisateur (Ctrl-C)
            initSignal ( SIGINT, finApplication );

            // Travail de fond du processus <Père>
            //   - Attente d'un signal (<SIGQUIT>, <SIGUSR2> ou <SIGINT>)
            //   - Pour <SIGUSR2> ou <SIGINT>, cela correspond à la fin de
            //     l'application multitâche
            //   - Pour <SIGQUIT>, on bascule l'état du processus <Tirage>
            for ( ; ; )
            {
                pause ( );
            }
        }
    }
    return 0;

} //----- fin de Main
