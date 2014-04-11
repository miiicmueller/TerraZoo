// Fonction d'acquisition du capteur de température
//
// Michael Mueller, Aubin Keumeuneuk, Cyrille Savy
//
// 21.03.2014

#include <stdio.h>
#include "contiki.h"
#include "dev/i2cmaster.h"
#include "dev/tmp102.h"
#include "gettmp.h"

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
int16_t gettmp()
    {
    int16_t tempint;
    int16_t raw;
    uint16_t absraw;
    int16_t sign;

    sign = 1;

    PRINTF("Reading Temp...\n");
    raw = tmp102_read_temp_raw();
    absraw = raw;
    if (raw < 0)
	{		// Perform 2C's if sensor returned negative data
	absraw = (raw ^ 0xFFFF) + 1;
	sign = -1;
	}
    tempint = (absraw >> 8) * sign;
    return (tempint);
    }
