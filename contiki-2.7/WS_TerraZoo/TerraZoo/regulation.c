// Tâche de régulation de température et de luminosité
//
// Michael Mueller, Aubin Keumeuneuk, Cyrille Savy
//
// 21.03.2014

#include "contiki.h"
#include <stdio.h>
#include "TZ_types.h"
#include "regulation.h"

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/**
 * process pour l'aquisition des capteurs
 */
PROCESS(p_regulation, "p_regulation");
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(p_regulation, ev, data)
  {
  //déclaration des variables
  static terraZooData_s* theDataStruct;

  PROCESS_BEGIN();
  PRINTF("[p_regulation] Process Begin\r\n");

  //Initialisations
  theDataStruct=NULL;


  while (1)
    {
    PROCESS_WAIT_EVENT_UNTIL(ev==P_REGULATION_START);
    PRINTF("[p_regulation] Restart\r\n");

    theDataStruct=(terraZooData_s*)data;
  
    //régulation de température
    if (theDataStruct->theTemp > theDataStruct->theTempConsigne)
	{
	theDataStruct->isHeaterOn=true;
	}
    else
	{
	theDataStruct->isHeaterOn=false;
	}

    PRINTF("[p_regulation] TempConsigne : %d\r\n", theDataStruct->theTempConsigne);
    PRINTF("[p_regulation] Temp : %d\r\n", theDataStruct->theTemp);
    PRINTF("[p_regulation] HeaterOn : %d\r\n", theDataStruct->isHeaterOn);

    //régulation de luminosité
    if (theDataStruct->theLight > theDataStruct->theLightConsigne)
	{
	theDataStruct->isLightOn=true;
	}
    else
	{
	theDataStruct->isLightOn=false;
	}
	
    PRINTF("[p_regulation] LightConsigne : %d\r\n", theDataStruct->theLightConsigne);
    PRINTF("[p_regulation] Light : %d\r\n", theDataStruct->theLight);
    PRINTF("[p_regulation] LightOn : %d\r\n", theDataStruct->isLightOn);
    
    }
    PROCESS_END();
  }

