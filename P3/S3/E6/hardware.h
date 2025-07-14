#ifndef HARDWARE_H
#define HARDWARE_H

#include "mbed.h"
#include "pinout.h"

// seven segment display anodes
// when in a int8_t, they are 0b-GFEDCBA
extern BusOut       g_seven_seg;

// display cathodes
extern PwmOut   g_dsl;
extern PwmOut   g_dsr;

// hc-sr04 range finder
extern DigitalOut   g_trg;
extern InterruptIn  g_ech;

// middle switch
extern InterruptIn  g_swm;

// hardware initialization
void hw_init(void);

#endif  // HARDWARE_H
