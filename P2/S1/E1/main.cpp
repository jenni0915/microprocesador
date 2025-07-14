#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static Ticker g_tick;

static bool volatile gb_toggle;
static void tick_isr (void) { gb_toggle=true; }

int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x40, 0x50, 0x54, 0x79, 0x71};
  
  return sseg[code];
  
} 
int main (void) {
  g_tick.attach(tick_isr, 4ms);
  g_dsr = 0;
  g_dsl = 1;
  for (;;) {
    if( gb_toggle ){
      gb_toggle = false;
      
      if(g_dsr ==1){
        g_dsr = 0;
        g_dsl = 1;
        g_seg = to_7seg(0);
      }
      else{
        g_dsl = 0;
        g_dsr = 1;
        g_seg = to_7seg(13);
      }
    }
    if(!gb_toggle){
    __WFI();
    }
  } // forever

} // main()

