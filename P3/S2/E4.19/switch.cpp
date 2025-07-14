#include "switch.h"

// extended state ------------------------------------------------------
  // "basic" state
  // complete this code to achieve the FSM functionality  ++++++++++++++
  typedef enum {SW_IDLE, SW_IN, SW_OUT} swm_state_t;
  static swm_state_t g_swm_state;
  //  ------------------------------------------------------------------

  // externally reachable objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
  bool           gb_swm_long_msg;
  bool           gb_swm_msg;
  bool volatile  gb_swm_can_sleep;
  //  ------------------------------------------------------------------

  // hardware resources
  // complete this code to achieve the FSM functionality  ++++++++++++++
  static Timer g_tmr;
  static InterruptIn *gp_swm;
  static Timeout g_tmo;
  
  //  ------------------------------------------------------------------

  // internal objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
  static bool          gb_swm_initd;
  static bool volatile gb_swm_rise;
  static bool volatile gb_swm_fall;
  static bool volatile gb_tmo;
  //  ------------------------------------------------------------------
// end of extended state  ----------------------------------------------

// ISRs ----------------------------------------------------------------
   // complete this code to achieve the FSM functionality  +++++++++++++
  static void g_swm_rise_isr(void) {
    gb_swm_rise=true;
    gb_swm_can_sleep = false;
  }
  static void g_swm_fall_isr(void) {
    gb_swm_fall=true;
    gb_swm_can_sleep = false;
  }
  static void gb_swm_initd_isr(void) {
    gb_swm_initd=true;
    gb_swm_can_sleep=false;
  }
  static void gb_tmo_isr(void){
    gb_tmo=true;
    gb_swm_can_sleep=false;
  }
   //  -----------------------------------------------------------------
// end of ISRs  --------------------------------------------------------

// FSM  ----------------------------------------------------------------
void swm_fsm (void) {
  if (gb_swm_initd) {  // protect against calling swm_fsm() w/o a previous call to swm_init()
    switch (g_swm_state) {
      // complete this code to achieve the FSM functionality  ++++++++++
      case SW_IN:
        if(gb_tmo){
          gb_tmo=false;
					if(gb_swm_fall==*gp_swm){
						g_tmr.reset();
            g_tmr.start();
            g_swm_state=SW_OUT;
					}//if(gb_swm_rise!=*gp_swm)
					else{
						g_swm_state=SW_IDLE;
					}
        }//if(gb_tmo)
				gb_swm_fall=false;
				gb_swm_rise=false;
      break;

      case SW_OUT:
        gb_swm_fall=false;
        gb_tmo=false;
        if(gb_swm_rise){
          gb_swm_rise=false;
          if(g_tmr.elapsed_time() < 1s){
            g_tmr.stop();
            gb_swm_msg=true;
						gb_swm_long_msg=false;
            g_swm_state=SW_IDLE;
          }else{
            g_tmr.stop();
            gb_swm_long_msg=true;
						gb_swm_msg=false;
            g_swm_state=SW_IDLE;
          }
        }
      break;
      
      default:
        gb_swm_rise=false;
        gb_tmo=false;
        if(gb_swm_fall){
          gb_swm_fall=false;
          g_tmo.attach(gb_tmo_isr,20ms);
          g_swm_state=SW_IN;
        }
      break;

      //  ----------------------------------------------------------------
    } // switch (swm_state)

  // complete this code to achieve the FSM functionality (can sleep?) ++




  //  ------------------------------------------------------------------
  } // if (gb_swm_initd)
}
// end of FSM ----------------------------------------------------------

// initialize FSM machinery  -------------------------------------------
void swm_init (InterruptIn *swm) {
  if (!gb_swm_initd) {
    gb_swm_initd = true;   // protect against multiple calls to swm_init

    // complete this code to achieve the FSM functionality  ++++++++++++

    // initialize state
    g_swm_state=SW_IDLE;
    gb_swm_rise=false;
    gb_swm_fall=false;
    gb_tmo=false;
    g_tmo.detach();
    // initial actions
    gp_swm = swm;              // save pointers to the pins
    gp_swm->mode(PullUp);
    // register ISRs
    gp_swm->rise(g_swm_rise_isr);
    gp_swm->fall(g_swm_fall_isr);

    //  ----------------------------------------------------------------
  }
}
// end of FSM initialization  ------------------------------------------
