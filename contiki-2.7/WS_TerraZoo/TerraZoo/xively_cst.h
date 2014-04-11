#ifndef XIVELY_CST_H
#define XIVELY_CST_H

#define NUMBER_OF_TERRA 3
#define NUMBER_OF_STREAM 4

// Variable passée à la compilation
#define TERRA_NUM 0

#define REQ_TYPE_PUT 0
#define REQ_TYPE_GET 1

#define STREAM_CONS_LUM 0
#define STREAM_CONS_TEMP 1
#define STREAM_SENS_LUM 2
#define STREAM_SENS_TEMP 3

/*
 gXivelyDevTab contient tout les feeds ID de xively

 770001174 = Terrarium 1.

 */

extern char* gXivelyDevTab[NUMBER_OF_TERRA];

/*
 gXivelyDevChan contient tout les cannaux à lire ou à écrire
 */
extern char* gXivelyDevChan[NUMBER_OF_STREAM];

/*
 gXivelyDevTab contient toutes les clés Api de xively

 KnQXl2W0m8psdBT0ljPbdBWwyWh1An1vZLr65xWNDf99HRdN = Terrarium 1.

 !! Mettre dans le même ordre que gXivelyDevTab[n] !!

 */
extern char* gXivelyDevApiKey[NUMBER_OF_TERRA];

/*
 gXivelyDevRequest contient toutes les requetes possibles
 */
extern char* gXivelyDevRequest[2];

#endif
