/**
 *
 * http_put.c
 *
 * Requête PUT sur le serveur xively
 *
 **/

#include "http_put.h"
#include "xively_cst.h"
#include "contiki.h"
#include "contiki-net.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include "net/uip-udp-packet.h"
//Pour test
#include "dev/i2cmaster.h"
#include "dev/tmp102.h"

#include <stdio.h>
#include <string.h>

#define TCP_SERVER_PORT 80
#define TCP_RETRY_NUM 5
#define START_TCP_TIME 9600
#define START_TCP_RECON 600

#define PRINTF(...) printf(__VA_ARGS__)

/**
 * Déclaration des diverses variables
 */

// Pointeur sur la connexion TCP
static struct uip_conn *conn;
static struct uip_conn *connXiv;

// Socket de communication par le noyeau
static struct psock gPSocketP80;

// Socket de communication par le noyeau
static struct psock gPSocketPN;

// Buffer TCP/IP
static char gBuffer[100];

// Buffer TCP/IP
static char gBuffer_Xiv[100];

//Stockage du port recu par la connexion TCP au port 80
static unsigned int gSharedPort;

// Variable de la machine d'état
static StateXivelyEnum gState = kStateWaitStart;

//Timer pour ne pas aller trop vite par rapport au Java
static struct etimer et;

/**
 * Foncitons statiques
 */
static int getNewTCPPort(struct psock *p);
static int handle_connection_xively(struct psock *p, char* pFeedID,
	char* pRequest, char* pDataStreamID, char* pApiKey, int16_t* pValue);

/*
 * Handle TCP pour recevoir le nouveau port de communication
 *
 * struct psock *p : Pointeur sur le socket
 *
 * Attention : On utilise des PROTOSOCKET et donc ces processus sont concurrents ! PT_THREAD
 */
static int getNewTCPPort(struct psock *p)
    {
    PSOCK_BEGIN(p)
    ;
    PSOCK_READTO(p, '\n');
    gSharedPort = (unsigned) atoi(gBuffer);
    PRINTF("Got: %u\r\n", gSharedPort);
PSOCK_END(p);
}

/*
 * Handle TCP pour lire ou écrire des inormations sur le serveur xively
 *
 * struct psock *p : Pointeur sur le socket
 * uint8_t* pFeedID : chaine de caractère du Feed
 * uint8_t* pRequest : chaine de caractère GET ou PUT
 * uint8_t* pDataStreamID : chaine de caractère du point final que l'on veut atteindre cf.xively
 * uint8_t* pValue : chaine de caractère de la valeur TODO : Convertir dans cette fonction
 *
 * Attention : On utilise des PROTOSOCKET et donc ces processus sont concurrents ! PT_THREAD
 */
static int handle_connection_xively(struct psock *p, char* pFeedID,
    char* pRequest, char* pDataStreamID, char* pApiKey, int16_t* pValue)
{

static char aValStr[10];

PSOCK_BEGIN(p)
;
//FEED ID  ex : 770001174
PSOCK_SEND_STR(p, pFeedID);
PSOCK_SEND_STR(p, "\r\n");
// API KEY
PSOCK_SEND_STR(p, pApiKey);
PSOCK_SEND_STR(p, "\r\n");
//REQ ex : PUT
PSOCK_SEND_STR(p, pRequest);
PSOCK_SEND_STR(p, "\r\n");
//Endpoint ex : sensor_lumin
PSOCK_SEND_STR(p, pDataStreamID);
PSOCK_SEND_STR(p, "\r\n");
// Value
sprintf(aValStr, "%d", *pValue);
PSOCK_SEND_STR(p, aValStr);
PSOCK_SEND_STR(p, "\r\n");

//Tester si il y a un retour (GET)
if (strcmp(pRequest, "GET") == 0)
    {
    //On va lire la valeur de retour
    PSOCK_READTO(p, '\n');
    if (strcmp(gBuffer_Xiv, "Error\r\n") == 0)
	{
	//On ne change pas la valeur
	PRINTF("Error !\r\n");
	}
    else
	{
	PRINTF("Got : %s\r\n", gBuffer_Xiv);
	*pValue = (unsigned) atoi(gBuffer_Xiv);
	PRINTF("Got : %p\r\n", *pValue);
	}
    }

PSOCK_END(p);
}

/**
 * Main process pour la communication TCP
 */

PROCESS(xively_access, "Xively Access");
AUTOSTART_PROCESSES(&xively_access);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD( xively_access, ev, data)
{
//Adresse IP du serveur
static uip_ipaddr_t server_addr;
//Adresse IP de la mote
static uip_ipaddr_t ipaddr;

// Itérateur du réessai de la connexion
static char aRetryNum = 0;

// Booléen si conntecté ou non
static char aConnected = 0;
static char aConnError = 0;

// Tableau de données Appstate pour la connexion TCP. Non utilisé
static char aAppStateMem[1];
static char aAppStateXivMem[1];

// Variable passée par le
static int16_t pValue = 0;

// Pour les requêtes
static uint8_t aReqType = 0;
static uint8_t aStreamType = 0;

static uint8_t aTemp = 0;

//Petite variable pour la démo
static uint8_t ind = 0;

// TODO Virer ces variables
int16_t raw;
uint16_t absraw;
int16_t sign;

PROCESS_BEGIN()
;

PRINTF("Setting server IP addressr\n");

// Adresse IP du serveur TCP sur la machine host tiré dur prefix recu par la connexion slip
// IP = aaaa::1
uip_ip6addr(&server_addr, 0xaaaa, 0, 0, 0, 0, 0, 0, 1);

// On règle notre addresse de base notre addresse IPv6
uip_ip6addr(&ipaddr, 0xaaaa, 0, 0, 0, 0, 0, 0, 0);
uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);

//gState = kStateWaitStart;
gState = kStatePutLumin;
// On veut obtenir un port de communication avec le serveur
tmp102_init();

while (1)
{
PRINTF("\nRestart.\n");
//TODO attente de la synchrtonisation
// pour l'instant on attend 60 secondes
etimer_set(&et, START_TCP_TIME);
PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

//Get temp
raw = tmp102_read_temp_raw();
sign = 1;
absraw = raw;
if (raw < 0)
    {		// Perform 2C's if sensor returned negative data
    absraw = (raw ^ 0xFFFF) + 1;
    sign = -1;
    }
pValue = (absraw >> 8) * sign;

PRINTF("Temp : %d\r\n", pValue);

// Init des variables
aConnected = 0;
aConnError = 0;
gSharedPort = 0;

/* Open a TCP connection to port 80 */
for (aRetryNum = 0; aRetryNum <= TCP_RETRY_NUM && !aConnected; aRetryNum++)
    {

    PRINTF("Connecting %d on port %d\r\n", aRetryNum, TCP_SERVER_PORT);

    conn = tcp_connect(&server_addr, UIP_HTONS(TCP_SERVER_PORT),
    NULL);

    if (conn == NULL)
	{
	PRINTF("Could not open TCP connection\r\n");
	continue;
	}

    do
	{
	PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	}
    while (!(uip_aborted() || uip_timedout() || uip_closed() || uip_connected()));

    if (uip_connected())
	{
	PRINTF("Connected\r\n");
	aConnected = 1;
	gSharedPort = 0;
	}
    else
	{
	PRINTF("Retrying...\r\n");
	aConnected = 0;
	}
    }

if (uip_connected())
    {
    PRINTF("P80 SOCK_INIT\r\n");
    PSOCK_INIT(&gPSocketP80, gBuffer, sizeof(gBuffer));

    // On récupère le port
    do
	{
	aTemp = getNewTCPPort(&gPSocketP80);
	PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

	// Si la connexion est coupée
	if ((uip_timedout() || uip_aborted() || uip_closed())
		&& (aTemp != PT_ENDED))
	    {
	    PRINTF("Connexion error...");
	    aConnError = 1;
	    }
	}
    while ((aTemp != PT_ENDED) && !aConnError); // On attend la fin du thread concurrent

    PSOCK_CLOSE(&gPSocketP80);
    //On ferme la connexion
    uip_abort();

    // We got the port
    /* Open a TCP connection to port aSharedPort */
    aConnected = 0;

    //On retest la connexion car elle peut avoir plantée
    if (!aConnError)
	{
	for (aRetryNum = 0; aRetryNum <= TCP_RETRY_NUM && !aConnected;
		aRetryNum++)
	    {

	    etimer_set(&et, START_TCP_RECON);
	    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	    PRINTF("Connecting %d on port %u \r\n", aRetryNum, gSharedPort);

	    connXiv = tcp_connect(&server_addr, UIP_HTONS(gSharedPort),
	    NULL);

	    if (connXiv == NULL)
		{
		PRINTF("Could not open TCP connection\r\n");
		continue;
		}

	    do
		{
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
		}
	    while (!(uip_aborted() || uip_timedout() || uip_closed()
		    || uip_connected()));

	    if (uip_connected())
		{
		PRINTF("Connected\r\n");
		aConnected = 1;
		gSharedPort = 0;
		}
	    else
		{
		PRINTF("Could not establish connection\r\n");
		PRINTF("Retrying...\r\n");
		aConnected = 0;
		}
	    }

	if (uip_connected())
	    {
	    PSOCK_INIT(&gPSocketPN, gBuffer_Xiv, sizeof(gBuffer_Xiv));

	    //On test la machine d'état
	    switch (gState)
		{
	    case kStatePutLumin:
		aReqType = REQ_TYPE_PUT;
		aStreamType = STREAM_SENS_LUM;
		gState = kStatePutTemp;
		break;
	    case kStatePutTemp:
		aReqType = REQ_TYPE_PUT;
		aStreamType = STREAM_SENS_TEMP;
		gState = kStateGetLuminCons;
		break;
	    case kStateGetLuminCons:
		aReqType = REQ_TYPE_GET;
		aStreamType = STREAM_CONS_LUM;
		gState = kStateGetTempCons;
		break;
	    case kStateGetTempCons:
		aReqType = REQ_TYPE_GET;
		aStreamType = STREAM_CONS_TEMP;
		gState = kStatePutLumin;
		break;
	    default:
		;
		}

	    do
		{
		aTemp = handle_connection_xively(&gPSocketPN,
			gXivelyDevTab[TERRA_NUM], gXivelyDevRequest[aReqType],
			gXivelyDevChan[aStreamType],
			gXivelyDevApiKey[TERRA_NUM], &pValue);
		PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
		// Si la connexion est coupée
		if ((uip_timedout() || uip_aborted() || uip_closed())
			&& (aTemp != PT_ENDED))
		    {
		    PRINTF("Connexion error...");
		    aConnError = 1;
		    }

		}
	    while ((aTemp != PT_ENDED) && !aConnError);

	    PSOCK_CLOSE(&gPSocketPN);

	    PRINTF("\nConnection closed.\n");
	    }
	}
    }
}

PROCESS_END();
}

