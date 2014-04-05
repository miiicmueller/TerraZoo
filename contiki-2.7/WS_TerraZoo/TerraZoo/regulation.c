// Tâche de calcul de la régulation du projet TerraZoo
//
// Michael Mueller, Aubin Keumeuneuk, Cyrille Savy
//
// 21.03.2014

#include "contiki.h"
#include <stdio.h>
#include "TZ_types.h"
#include "regulation.h"

#define PRINTF(...) printf(__VA_ARGS__)

/**
 * process pour l'aquisition des capteurs
 */
PROCESS(p_regulation, "regulation");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(p_regulation, ev, data)
  {
  //déclaration des variables
  static terraZooData_s* theDataStruct;
  static int16_t aTemperature;
  static int16_t aLight;

  PROCESS_BEGIN();
  PRINTF("[regulation] Process Begin\r\n");

  //Initialisations
  theDataStruct = NULL;
  aTemperature = 0;
  aLight = 0;

  while (1)
    {
    PROCESS_WAIT_EVENT_UNTIL(ev==P_REGULATION_START);
    PRINTF("[regulation] Restart\r\n");

    //Compute temp
    theDataStruct=(terraZooData_s*)data;
    theDataStruct->isHeaterOn = !theDataStruct->isHeaterOn;


    //Compute light
    theDataStruct=(terraZooData_s*)data;
    theDataStruct->isLightOn = !theDataStruct->isLightOn;
    }

    PROCESS_END();
  }