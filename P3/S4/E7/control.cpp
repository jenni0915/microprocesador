#include "control.h"
#include "display.h"
#include "switch.h"

// extended state ------------------------------------------------------
  // "basic" state
  // complete this code to achieve the FSM functionality  ++++++++++++++
  typedef enum {CTR_OFF,CTR_ON} ctr_state_t;
  typedef enum {CTR_LI, CTR_DI, CTR_LE, CTR_OF} mode_state_t;
  static ctr_state_t g_ctr_state;
  static mode_state_t g_mode_state;
  
   // externally reachable objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
  bool volatile gb_ctr_can_sleep;
  bool          gb_changeMode_msg;
  
  
  //  ------------------------------------------------------------------
  
  // hardware resources
  // complete this code to achieve the FSM functionality  ++++++++++++++
  static InterruptIn *g_swm;
  static BufferedSerial *g_pc;
  static DigitalOut *g_dsr;
  
  // internal objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
  static bool   gb_ctr_initd;
  static bool   gb_ctr_short_evnt;
  static TimeOut g_to;
    
  // ISRs ----------------------------------------------------------------
   // complete this code to achieve the FSM functionality  +++++++++++++
  static void g_ctr_short_isr(void){
    gb_ctr_short_evnt = true;
  }

	// FSM  ----------------------------------------------------------------
void ctr_fsm (void){
  if(gb_ctr_initd){
    switch (g_ctr_state){
      case CTR_ON:
        //gb_display_on_msg = false;
        gb_display_update_msg = true;
        g_display_brightness = 99;
        gb_display_brightness_msg = true;
      
        if(gb_swm_msg){
          gb_swm_msg = false;
          *g_dsr = 1;
          g_to.attach(g_ctr_short_isr,50ms);
          if(gb_ctr_short_evnt){
            gb_ctr_short_evnt = false;
            *g_dsr = 0;
          }
        }
        
      
      
      break;
      
      default:
        
      
      break;
    }
  }

}

