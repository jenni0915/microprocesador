#include "mbed.h"
#include "pinout.h"
#include "hardware.h"
#include "to_7seg.h"
#include "range_finder.h"
#include "switch.h"
#include "display.h"

// mux stuff
static Ticker         g_mux_tick;
static bool volatile  gb_mux_evnt;

static void mux_isr (void) {
  gb_mux_evnt = true;
}

// range_finder automatic start
static Ticker         g_meas_tick;
static bool volatile  gb_meas_evnt;
static Timeout        g_meas_to;

static void meas_isr (void) {
  gb_meas_evnt = true;
}

int main (void) {
  // right display on?
  bool      b_right = false;
  // auto measure?
  bool      b_auto_measure = false;
  // the 4 MSB of this variable hold the symbol to be displayed at the
  //  left display, the 4 LSB that to be displayed at the right one
  uint8_t   disp = 0;

  hw_init();

  g_seven_seg = 0;
  g_dsr = b_right;
  g_dsl = !b_right;
  g_mux_tick.attach(mux_isr, 4ms);          // 250 Hz

  // initialize the range finder FSM
  rf_init(&g_trg, &g_ech);

  // initialize the middle switch FSM
  // complete this code  +++++++++++++++++++++++++++++++++++++++++++++++
  swm_init(&g_swm);
  //  ------------------------------------------------------------------
  // initialize the middle switch FSM
  display_init(&g_dsl, &g_dsr, &g_seven_seg);
  
  for (;;) {
    // the range finder FSM
    rf_fsm();
    //g_meas_tick.attach(meas_isr, 100ms);      // 10 Hz
    
    // the middle switch FSM
    // complete this code  +++++++++++++++++++++++++++++++++++++++++++++
    swm_fsm();
    
    //the display FSM
    display_fsm();
    
    // complete this code  (switch long & short pulsation actions) +++++
    if(gb_swm_msg){
      gb_swm_msg = false;
      g_meas_to.attach(meas_isr,100ms); 
      disp = g_rf_range_cm;
    }
    
    if(gb_swm_long_msg){
      gb_swm_long_msg = false;
      if(b_auto_measure){
        b_auto_measure = false;
        g_meas_tick.attach(meas_isr,100ms);
        disp = g_rf_range_cm;
      }else{
        b_auto_measure = true;
        g_meas_tick.detach();
        disp = g_rf_range_cm;
      }
    }

    //  ----------------------------------------------------------------

    // start a new range measurement every 100 ms
    if (gb_meas_evnt) {
      gb_meas_evnt = false;
      gb_rf_start_msg = true;
    }

    // when the measurement is complete, update variable disp
    if (gb_rf_done_msg) {
      gb_rf_done_msg = false;
      
      if (g_rf_range_cm > 99) {
        disp = 0xBB;  // --
        g_display_segs = disp;
        g_display_brightness = disp;
      } else if (-1 == g_rf_range_cm) {
        disp = 0xEC;  // Er
        g_display_segs = disp;
        g_display_brightness = disp;
      } else {
        disp = (g_rf_range_cm / 10) << 4 | (g_rf_range_cm % 10);
        g_display_segs = disp;
        g_display_brightness = disp;
      }
    }

    // display multiplex
    if (gb_mux_evnt) {
      // complete this code for the display multiplex ++++++++++++++++++
      gb_mux_evnt = false;
      gb_display_on_msg = true;
      // ---------------------------------------------------------------
    }

    // sleep
    if (!gb_meas_evnt && !gb_rf_done_msg && !gb_mux_evnt
        && !gb_rf_start_msg && gb_rf_can_sleep
        && !gb_swm_msg && !gb_swm_long_msg && gb_swm_can_sleep
        && gb_display_can_sleep && !gb_display_off_msg && !gb_display_on_msg) {
      __WFI();
    }
    __enable_irq();
  } // forever
} // main()

