/**
 *
 * Xively_cst.c
 *
 * Contient les constantes nécessaires pour acceder au serveur xively
 *
 **/

#include "xively_cst.h"

/*
 gXivelyDevTab contient tout les feeds ID de xively

 770001174 = Terrarium 1.

 */

char* gXivelyDevTab[NUMBER_OF_TERRA] =
    {
    "210248330", "1082788112", "1539107217"
    };

/*
 gXivelyDevChan contient tout les cannaux à lire ou à écrire
 */
char* gXivelyDevChan[NUMBER_OF_STREAM] =
    {
    "consigne_lumin", "consigne_temp", "sensor_lumin", "sensor_temp"
    };

/*
 gXivelyDevTab contient toutes les clés Api de xively

 KnQXl2W0m8psdBT0ljPbdBWwyWh1An1vZLr65xWNDf99HRdN = Terrarium 1.

 !! Mettre dans le même ordre que gXivelyDevTab[n] !!

 */
char* gXivelyDevApiKey[NUMBER_OF_TERRA] =
    {
	    "LJ4ZXbwj82zTbqXAuHVEp8pROxTTaMfLLxj1tuD0DMxTtL3N",
	    "Atg2i5U3nedPkfD6gTI8VacoZiKw2YyIupXf9nvGME4KIUpx",
	    "R5z9dd4uQ15PSauuF68vTuntYGibKNoc2UnqBXW5Hyr2rbC2"
    };

/*
 gXivelyDevRequest contient toutes les requetes possibles
 */
char* gXivelyDevRequest[2] =
    {
    "PUT", "GET"
    };
