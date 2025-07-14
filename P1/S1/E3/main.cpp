#include "mbed.h"
#include "pinout.h"
#include "sw_tick_serial.h"

// leds
static DigitalOut g_ldl(LDL_PIN);
static DigitalOut g_ldm(LDM_PIN);
static DigitalOut g_ldr(LDR_PIN);

int main (void) {
  sw_tick_serial_init();
  g_ldl = 0;
  g_ldm = 0;
  g_ldr = 1;

  for (;;) {
    if (gb_tick_1s_evnt) {
      gb_tick_1s_evnt = false;
      bool volatile tmp = g_ldl;
      g_ldl = g_ldm;
      g_ldm = g_ldr;
      g_ldr = tmp;
    } // if (gb_tick_1s_evnt)
  } // forever
} // main()
