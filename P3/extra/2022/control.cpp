#include "control.h"

// extended state ------------------------------------------------------
  // "basic" state
  // complete this code to achieve the FSM functionality  ++++++++++++++
  typedef enum {CTR_START,CTR_TESTING, CTR_RESULT} ctr_state_t;
  static ctr_state_t g_ctr_state;
  
   // externally reachable objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
  bool volatile gb_ctr_can_sleep;
  bool          ;
  
  
  //  ------------------------------------------------------------------
  
  // hardware resources
  // complete this code to achieve the FSM functionality  ++++++++++++++
  static PwmOut  *g_dsl;
  static PwmOut  *g_dsr;
  static BusOut  *g_seven_seg;
  static BusOut  *g_leds;
  
  // internal objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
  static bool   gb_ctr_initd;
  uint8_t  g_rf_count;


void ctr_fsm(void){
  if(gb_ctr_initd){
    switch(g_ctr_state){
      case CTR_TESTING:
        
      
      break;
      case CTR_RESULT:
        
      
      break;
      default:
        
      
      
      break;
    
    }
  
  }
}