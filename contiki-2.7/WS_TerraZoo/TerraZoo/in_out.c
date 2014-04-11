// Tâche d'acquisition des capteurs et d'écritures des actionneurs du projet TerraZoo
//
// Michael Mueller, Aubin Keumeuneuk, Cyrille Savy
//
// 21.03.2014

#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
#include "TZ_types.h"
#include "in_out.h"
#include "getlight.h"
#include "gettmp.h"

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/**
 * process pour l'aquisition des capteurs
 */
PROCESS(p_inputs, "p_inputs");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(p_inputs, ev, data)
  {
  //déclaration des variables
  static terraZooData_s* theDataStruct;
  static int16_t aTemperature;
  static int16_t aLight;


  PROCESS_BEGIN();
  PRINTF("[p_inputs] Process Begin\r\n");

  //Initialisations
  theDataStruct=NULL;
  aTemperature=0;
  aLight=0;


  while (1)
    {
    PROCESS_WAIT_EVENT_UNTIL(ev==P_IN_START);
    PRINTF("[p_inputs] Restart\r\n");
  
    theDataStruct=(terraZooData_s*)data;

    //Get temp
    aTemperature=gettmp();
    theDataStruct->theTemp=aTemperature;

    PRINTF("[p_inputs] Temp : %d\r\n", theDataStruct->theTemp);

    //Get light
    aLight=500;//getlight();
    theDataStruct->theLight=aLight;

    PRINTF("[p_inputs] Light : %d\r\n", aLight);
    }

    PROCESS_END();
  }
  
 /**
 * process pour l'écriture des sorties (actionneurs)
 */
PROCESS(p_outputs, "p_outputs");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(p_outputs, ev, data)
  {
  //déclaration des variables
  static terraZooData_s* theDataStruct;
  static int16_t aTemperature;
  static int16_t aLight;


  PROCESS_BEGIN();
  PRINTF("[p_outputs] Process Begin\r\n");

  //Initialisations
  theDataStruct = NULL;
  aTemperature = 0;
  aLight = 0;

  while (1)
    {
    PROCESS_WAIT_EVENT_UNTIL(ev==P_OUT_START);
    PRINTF("[p_outputs] Restart\r\n");

    theDataStruct=(terraZooData_s*)data;

    //write heater temp
    if(theDataStruct->isHeaterOn==true)
      {
      leds_on(LEDS_RED);
      }
    else
      {
      leds_off(LEDS_RED);
      }

    PRINTF("[p_outputs] Write heater\r\n");


    //write light
    if(theDataStruct->isLightOn==true)
      {
      leds_on(LEDS_BLUE);
      }
    else
      {
      leds_off(LEDS_BLUE);
      }
    PRINTF("[p_outputs] Write Light\r\n");
    }

    PROCESS_END();
  }