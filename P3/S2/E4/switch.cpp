#include "switch.h"

// extended state ------------------------------------------------------
  // "basic" state
  // complete this code to achieve the FSM functionality  ++++++++++++++
typedef enum {SW_IDLE, SW_IN, SW_OUT} swm_state_t;
static swm_state_t g_swm_state;

  //  ------------------------------------------------------------------

  // externally reachable objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
bool gb_swm_long_msg;
bool gb_swm_msg;
bool volatile  gb_swm_can_sleep;


  //  ------------------------------------------------------------------

  // hardware resources
  // complete this code to achieve the FSM functionality  ++++++++++++++
static InterruptIn *gp_swm;
static Timer g_swm_tmr;
static Timeout g_swm_to;

  //  ------------------------------------------------------------------

  // internal objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
static bool           gb_swm_initd;
static bool volatile  gb_swm_rise_evnt;   // rise on echo event
static bool volatile  gb_swm_fall_evnt;   // fall on echo event
static bool volatile  gb_swm_to_evnt;     // timeout elapsed event




  //  ------------------------------------------------------------------
// end of extended state  ----------------------------------------------

// ISRs ----------------------------------------------------------------
   // complete this code to achieve the FSM functionality  +++++++++++++

static void swm_rise_isr(void){
  gb_swm_rise_evnt = true;
  gb_swm_can_sleep = false;
}
static void swm_fall_isr(void){
  gb_swm_fall_evnt = true;
  gb_swm_can_sleep = false;
}
static void swm_to_isr(void) {
  gb_swm_to_evnt = true;
  gb_swm_can_sleep = false;
}


   //  -----------------------------------------------------------------
// end of ISRs  --------------------------------------------------------

// FSM  ----------------------------------------------------------------
void swm_fsm (void) {
  if (gb_swm_initd) {  // protect against calling swm_fsm() w/o a previous call to swm_init()
    
    switch (g_swm_state) {
      // complete this code to achieve the FSM functionality  ++++++++++
      case SW_IN:
        gb_swm_rise_evnt = false;
        gb_swm_fall_evnt = false;
      
        if(gb_swm_to_evnt){
          gb_swm_to_evnt = false;
          if( gb_swm_fall_evnt == *gp_swm){
            g_swm_tmr.reset();
            g_swm_tmr.start();
            g_swm_state = SW_OUT;
          }else {
            g_swm_state = SW_IDLE;
          }
        }
      break;
      
      case SW_OUT:
        gb_swm_fall_evnt = false;
        gb_swm_to_evnt = false;
      
        if(gb_swm_rise_evnt){
          gb_swm_rise_evnt = false;
          if(g_swm_tmr.elapsed_time() < 1s){
            g_swm_tmr.stop();
            gb_swm_msg = true;
            g_swm_state = SW_IDLE;
          }else {
            g_swm_tmr.stop();
            gb_swm_long_msg = true;
            g_swm_state = SW_IDLE;
          }
        }
      break;
      
      default:
        gb_swm_rise_evnt = false;
        gb_swm_to_evnt = false;
        if(gb_swm_fall_evnt){
          gb_swm_fall_evnt = false;
          g_swm_state = SW_IN;
          g_swm_to.attach(swm_to_isr, 20ms);
        }





      //  ----------------------------------------------------------------
    } // switch (swm_state)

  // complete this code to achieve the FSM functionality (can sleep?) ++
  __disable_irq();
    if(!gb_swm_rise_evnt && !gb_swm_fall_evnt && !gb_swm_to_evnt){
      gb_swm_can_sleep = true;
    }
  __enable_irq();


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
    g_swm_state = SW_IDLE;
    gb_swm_rise_evnt = false;
    gb_swm_fall_evnt = false;
    g_swm_to.detach();
    gb_swm_to_evnt = false;


    // initial actions
    gp_swm = swm;              // save pointers to the pins
    gp_swm->mode(PullUp);
    // register ISRs
    gp_swm->rise(swm_rise_isr);
    gp_swm->fall(swm_fall_isr);

    //  ----------------------------------------------------------------
  }
}
// end of FSM initialization  ------------------------------------------
