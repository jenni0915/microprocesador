#include "pinout.h"
#include "sw_tick_serial.h"
#include "to_7seg.h"

static BusOut g_leds(LDL_PIN,LDM_PIN,LDR_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static BusOut display(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);

extern int8_t to_7seg(int8_t code);

int main (void) {
  sw_tick_serial_init();
  int contador=0;
  g_dsl=0;
  g_dsr=1;
  g_leds=1;
  for (;;) {
    if(contador>9){contador=0;}
    if(gb_swl_evnt){
      gb_swl_evnt=false;
      display=to_7seg(contador);
      contador++;

    }//if(sw_L)
    
    if(gb_swr_evnt){
      gb_swr_evnt=false;
      g_leds=((4==g_leds)?1:(g_leds<<1));
    }//if(sw_R)
    
    if(gb_swm_evnt){
      gb_swm_evnt=false;
      if(g_dsr==1){
        g_dsr=0;
        g_dsl=1;
      }else{
        g_dsr=1;
        g_dsl=0;
      }
    }//if(sw_M)
  } // forever
} // main()

