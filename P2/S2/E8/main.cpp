#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static InterruptIn g_swr(SWR_PIN);
static InterruptIn g_swl(SWL_PIN);
static InterruptIn g_swm(SWM_PIN);

static Timeout g_swr_to;
static Timeout g_swl_to;
static Timeout g_swm_to;

static Ticker g_tick;
static bool volatile gb_toggle;
static bool volatile gb_swr_to;
static bool volatile gb_swl_to;
static bool volatile gb_swm_to;

static bool volatile gb_swr_fall_evnt;
static bool volatile gb_swr_rise_evnt;
static bool volatile gb_swl_fall_evnt;
static bool volatile gb_swl_rise_evnt;
static bool volatile gb_swm_fall_evnt;
static bool volatile gb_swm_rise_evnt;

static void swr_to_isr (void){
  gb_swr_to = true;
}
static void swl_to_isr (void){
  gb_swl_to = true;
}
static void swm_to_isr (void){
  gb_swm_to = true;
}
static void swr_fall_isr (void) {
  gb_swr_fall_evnt = true;
}
static void swr_rise_isr (void) {
  gb_swr_rise_evnt = true;
}
static void tick_isr (void) {
  gb_toggle = true;
}
static void swl_fall_isr (void) {
 gb_swl_fall_evnt = true;
}
static void swl_rise_isr (void) {
 gb_swl_rise_evnt = true;
}
static void swm_fall_isr (void) {
 gb_swm_fall_evnt = true;
}
static void swm_rise_isr (void) {
 gb_swm_rise_evnt = true;
}
int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  return sseg[code];
} 

int main (void) {
  int contador = 0 ;
  bool b_swr_state = false;
  bool b_swl_state = false;
  bool b_swm_state = false;
  
  bool b_right = false;
 
  g_swr.mode(PullUp);
  g_swr.fall(swr_fall_isr);
  g_swr.rise(swr_rise_isr);
  
  g_swl.mode(PullUp);
  g_swl.fall(swl_fall_isr);
  g_swl.rise(swl_rise_isr);
  g_swm.mode(PullUp);
  g_swm.fall(swl_fall_isr);
  g_swm.rise(swl_rise_isr);

  g_tick.attach(tick_isr,4ms);
  g_dsr = b_right;
  g_dsl = !b_right;
  g_seg = to_7seg(b_right ? contador % 10 : contador / 10);

  for (;;) {

    if( gb_toggle ){
      gb_toggle = false;
      b_right = !b_right;
      g_dsr = b_right;
      g_dsl = !b_right;
      g_seg = to_7seg(b_right ? contador % 10 : contador / 10);
    }
    if( gb_swr_to ){
      gb_swr_to = false;
      b_swr_state = !b_swr_state;
    }
    if( gb_swl_to ){
      gb_swl_to = false;
      b_swl_state = !b_swl_state;
    }
    if( gb_swm_to ){
      gb_swm_to = false;
      b_swm_state = !b_swm_state;
    }
    if( gb_swr_fall_evnt ){ 
      gb_swr_fall_evnt = false;
      if(!b_swr_state ){
        contador += (contador >= 99) ? -contador : 1 ;
        g_swr_to.attach(swr_to_isr,10ms);
      }
     }
    if( gb_swr_rise_evnt ){
      gb_swr_rise_evnt = false;
      if(b_swr_state ){
        g_swr_to.attach(swr_to_isr,10ms);
      }
    }
    if(gb_swl_fall_evnt){
      gb_swl_fall_evnt = false;
      if(!b_swl_state ){
        contador += ((contador > 0) ? -1 : 0);
        g_swl_to.attach(swl_to_isr,10ms);
      }
    }
    if (gb_swl_rise_evnt) {
      gb_swl_rise_evnt = false;
      if(b_swl_state ){
        g_swl_to.attach(swl_to_isr,10ms);
      }
     }
    if(gb_swm_fall_evnt){
      gb_swm_fall_evnt = false;
      if(!b_swm_state ){
        contador = 99-contador ;
        g_swm_to.attach(swm_to_isr,10ms);
      }
    }
    if (gb_swm_rise_evnt) {
      gb_swm_rise_evnt = false;
      if(b_swm_state ){
        g_swm_to.attach(swm_to_isr,10ms);
      }
     }
    

     
    __disable_irq();
     if(!gb_toggle && !gb_swr_rise_evnt && !gb_swr_fall_evnt && !gb_swl_rise_evnt 
       && !gb_swl_rise_evnt && !gb_swm_rise_evnt && !gb_swm_fall_evnt){
      __WFI();
    }
    __enable_irq();
    }// forever
 
}  // main()


