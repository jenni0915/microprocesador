#include "mbed.h"
#include "pinout.h"
#include "sw_tick_serial.h"

// leds
static BusOut g_leds(LDR_PIN,LDM_PIN, LDL_PIN);

int main (void) {
  sw_tick_serial_init();
  g_leds=1;
  for (;;) {
    if (gb_tick_100ms_evnt) {
      gb_tick_100ms_evnt = false;
      g_leds=((4==g_leds)? 1 : (g_leds<<1));
    } // if (gb_tick_1s_evnt)
  } // forever
} // main()
