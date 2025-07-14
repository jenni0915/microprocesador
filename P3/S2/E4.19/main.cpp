#include "mbed.h"
#include "pinout.h"
#include "hardware.h"
#include "to_7seg.h"
#include "range_finder.h"
#include "switch.h"

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

  for (;;) {
    // the range finder FSM
    rf_fsm();
    // the middle switch FSM
    // complete this code  +++++++++++++++++++++++++++++++++++++++++++++
    swm_fsm();
    //  ----------------------------------------------------------------


    // complete this code  (switch long & short pulsation actions) +++++
    if(gb_swm_long_msg){
      gb_swm_long_msg = false;
      b_auto_measure=!b_auto_measure;
      if(b_auto_measure){
				g_meas_tick.attach(meas_isr,100ms); 
				disp=g_rf_range_cm;
				g_meas_to.detach();
      } //if(b_auto_measure)
			else{
        g_meas_tick.detach();
        disp=g_rf_range_cm;
				g_meas_to.attach(meas_isr,100ms);
      }//else
    } //if(gb_swm_long_msg)
		if(gb_swm_msg){
      gb_swm_msg=false;
			g_meas_to.attach(meas_isr,100ms);
      disp = g_rf_range_cm;
    }//if(gb_swm_msg)

    
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
      } else if (-1 == g_rf_range_cm) {
        disp = 0xEC;  // Er
      } else {
        disp = (g_rf_range_cm / 10) << 4 | (g_rf_range_cm % 10);
      }
    }

    // display multiplex
    if (gb_mux_evnt) {
      // complete this code for the display multiplex ++++++++++++++++++
      gb_mux_evnt = false;
			if(g_dsr){
				g_dsr=0;
				g_seven_seg = to_7seg(disp>>4);
				g_dsl=1;
			}//if(g_dsr)
			else if(g_dsl){
				g_dsl=0;
				g_seven_seg = to_7seg(disp&0x0f);
				g_dsr=1;
			}//else if(g_dsl)
			else{
				//nada
			}//else
      // ---------------------------------------------------------------
    }

    // sleep
    __disable_irq();
    if (!gb_meas_evnt && !gb_rf_done_msg && !gb_mux_evnt
        && !gb_rf_start_msg && gb_rf_can_sleep
        && !gb_swm_msg && !gb_swm_long_msg && gb_swm_can_sleep) {
      __WFI();
    }
    __enable_irq();
  } // forever
} // main()

