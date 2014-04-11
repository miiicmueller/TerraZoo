// Fonction d'acquisition du capteur de luminosité
//
// Michael Mueller, Aubin Keumeuneuk, Cyrille Savy
//
// 21.03.2014

#include <stdio.h>
#include "contiki.h"
#include "dev/i2cmaster.h"
#include "dev/light-ziglet.h"

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


uint16_t getlight()
    {

    uint16_t light;

    PRINTF("[getlight] read");
    light = light_ziglet_read();
    
    return light;
    }

