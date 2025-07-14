#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static InterruptIn g_swl(SWL_PIN);
static BusOut g_leds(LDR_PIN, LDM_PIN, LDL_PIN);

static Timeout g_to;
static Ticker g_tick;
static bool volatile gb_swl_fall_evnt;
static bool volatile gb_dbnce_evnt;
static bool volatile gb_swl_rise_evnt;
static bool volatile gb_tick;
static void swl_fall_isr (void) {gb_swl_fall_evnt = true;}
static void swl_rise_isr (void) {gb_swl_rise_evnt = true;}
static void tick_isr (void) {gb_tick = true;}
static void to_dbnce_isr (void) {gb_dbnce_evnt = true; }
uint8_t g_cnt_sw;
int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  return sseg[code];
} 
int main (void) {
  g_dsl = 1;
  g_dsr = 1;
  g_leds = 0;
  g_cnt_sw = 0;
  bool    b_swr_state = false;
  g_tick.attach(tick_isr, 10ms);
  g_swl.mode(PullUp);
  g_swl.fall(swl_fall_isr);
  g_swl.rise(swl_rise_isr);

  for (;;) {
    if(gb_swl_fall_evnt){
      gb_swl_fall_evnt = false;
        g_to.attach(to_dbnce_isr, 10ms);
        
    }
    if(gb_swl_rise_evnt){
      gb_swl_rise_evnt = false;
      g_to.attach(to_dbnce_isr, 10ms);
        
    }
    if(gb_dbnce_evnt){
      gb_dbnce_evnt = false;
      b_swr_state = g_swl.read();
      if(!b_swr_state){
        g_cnt_sw += (99 > g_cnt_sw) ? 1 : -99;
      }

      g_leds = g_cnt_sw;
      g_seg = (1 == g_dsl) ? to_7seg(g_cnt_sw/10) : to_7seg(g_cnt_sw%10);
    }
    if(gb_tick){
      gb_tick = false;
      if(g_dsl == 1){
        g_dsl = 0;
        g_dsr = 1;
      }else if(g_dsr == 1){
        g_dsl = 1;
        g_dsr = 0;
      }
      g_seg = (1 == g_dsl) ? to_7seg(g_cnt_sw/10) : to_7seg(g_cnt_sw%10);

    }
  __disable_irq();
  if(!gb_swl_fall_evnt && !gb_swl_fall_evnt && !gb_tick){
    __WFI();
  }
  __enable_irq();
  }// forever
}  // main()


