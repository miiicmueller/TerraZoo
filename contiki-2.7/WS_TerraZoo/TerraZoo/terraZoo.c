// Progamme principal du projet TerraZoo
//
// Michael Mueller, Aubin Keumeuneuk, Cyrille Savy
//
// 21.03.2014

#include "contiki.h"
#include <stdio.h>
//#include "gettmp.h"
#include "http_put.h"
#include "sys/clock.h"
#include "TZ_types.h"
#include "sys/etimer.h"

#define MAIN_PROCESS_PERIOD CLOCK_SECOND*60

#define PRINTF(...) printf(__VA_ARGS__)

//définition de variables globales


//prototype du main_process
PROCESS(main_process, "main_process");

//démarrage des processus
AUTOSTART_PROCESSES(&main_process);

//déclaration du main_process
PROCESS_THREAD( main_process, ev, data)
{
  //déclaration des variables du main process
  static struct etimer et;
  static terraZooData_s theTerraZooData;
 
  
  //début du main process
  PROCESS_BEGIN();
  PRINTF("[main_process] BEGIN\r\n");
  
  //initialisation des variables du main process
  theTerraZooData.theTemp=0;
  
  //démarrage des process secondaires
  process_start(&xively_access, NULL);
  
  
  //première initialisation du timer de synchronisation des process
  etimer_set(&et, MAIN_PROCESS_PERIOD);
  
  while(1)
  {
  //relancemement du timer (timer relancé après l'avoir lu pour que le temps de boucle soit stable)
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  etimer_set(&et, MAIN_PROCESS_PERIOD);
  PRINTF("[main_process] new loop\r\n");
  
  //acquisition des données des capteurs
  theTerraZooData.theTemp++;
  PRINTF("[main_process] Temp : %d\r\n", theTerraZooData.theTemp);
  //process_post_synch(&mote_data_input, DATA_INPUT_START, &theTerraZooData);
  
  //régulation des sorties en fonctions des données des capteurs et des consignes
  //process_post_synch(&mote_reg_output, REG_OUTPUT_START, &theTerraZooData);
  
  //envoi des données sur le serveur Xively
  process_post_synch(&xively_access, XIVELY_ACCESS_START, &theTerraZooData);  
  }
  PROCESS_END();
}



