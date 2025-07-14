# include "display.h"
# include "to_7seg.h"

// extended state ------------------------------------------------------
  // "basic" state
  // complete this code to achieve the FSM functionality  ++++++++++++++
typedef enum {SW_ON,SW_OFF} display_state_t ;;
static display_state_t g_display_state;
  //  ------------------------------------------------------------------

  // externally reachable objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
bool           gb_display_on_msg;
bool           gb_display_off_msg;
bool           gb_display_update_msg;
uint16_t       g_display_segs; 
bool           gb_display_brightness_msg;
uint8_t        g_display_brightness; 
bool volatile  gb_display_can_sleep;
  //  ------------------------------------------------------------------

  // hardware resources
  // complete this code to achieve the FSM functionality  ++++++++++++++
static PwmOut  *g_dsl;
static PwmOut  *g_dsr;
static BusOut  *g_seven_seg;

  //  ------------------------------------------------------------------
static Ticker   g_mux_tick;
  // internal objects
  // complete this code to achieve the FSM functionality  ++++++++++++++
static bool          gb_display_initd;
static bool          gb_right;
static bool volatile gb_mux_evnt;
  //  ------------------------------------------------------------------
// end of extended state  ----------------------------------------------

// ISRs ----------------------------------------------------------------
   // complete this code to achieve the FSM functionality  +++++++++++++
static void mux_isr (void) {
  gb_mux_evnt = true;
}

   //  -----------------------------------------------------------------
// end of ISRs  --------------------------------------------------------

// FSM  ----------------------------------------------------------------

  // complete this code to achieve the FSM functionality (can sleep?) ++
void display_fsm(void){
  if(gb_display_initd){
    switch(g_display_state){
      case SW_ON:
        gb_display_on_msg = false;
        if(gb_display_brightness_msg || gb_display_update_msg){
          gb_display_brightness_msg = false;
          gb_display_update_msg = false;
            if(gb_mux_evnt){
              gb_mux_evnt = false;
              gb_right = !gb_right;
              if(gb_right){
                gb_right = false;
                g_dsl ->pulsewidth_us(0);
                *g_seven_seg = to_7seg(g_display_segs & 0x0fu);
                g_dsr ->pulsewidth_us(g_display_brightness & 0xff);
              }else{
                gb_right = true;
                g_dsr ->pulsewidth_us(0);
                *g_seven_seg = to_7seg(g_display_segs >> 4);
                g_dsr ->pulsewidth_us(g_display_brightness & 0xffu);
              }
            }
            g_display_state = SW_ON;
          }
        if(gb_display_off_msg){
          gb_display_off_msg = false;
          g_display_state = SW_OFF;
          g_mux_tick.detach();
          
        }
      default:
        gb_display_brightness_msg = false;
        gb_display_update_msg = false;
        gb_display_off_msg = false;
        if(gb_display_on_msg){
          gb_display_on_msg = false;
          g_mux_tick.attach(mux_isr,4ms);
          g_display_state = SW_ON;
        }
      break;
    }
  }
  __disable_irq();
  if(!gb_display_update_msg && !gb_display_brightness_msg && !gb_mux_evnt){
    gb_display_can_sleep=true;
  }//sleep
  __enable_irq();
}

  //  ------------------------------------------------------------------

// end of FSM ----------------------------------------------------------

// initialize FSM machinery  -------------------------------------------
void display_init(PwmOut *dsl, PwmOut *dsr, BusOut *seven_seg){
  if(!gb_display_initd){
    gb_display_initd = true;
    g_display_state = SW_OFF;
    gb_right = false;
    g_mux_tick.detach();
    g_dsl = dsl;
    g_dsr = dsr;
    g_seven_seg = seven_seg;
    g_dsl->period_us(100);
    g_dsr->period_us(100);
    g_dsl->pulsewidth_us(100);
    g_dsr->pulsewidth_us(100);
  }
}
// end of FSM initialization  ------------------------------------------
