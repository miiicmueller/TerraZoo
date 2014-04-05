#ifndef IN_OUT_H
#define IN_OUT_H

#include "contiki.h"

PROCESS_NAME(p_inputs);
PROCESS_NAME(p_outputs);

#define P_IN_START (process_event_t)1
#define P_OUT_START (process_event_t)1

#endif