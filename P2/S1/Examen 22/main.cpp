#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static DigitalOut g_ldl(LDL_PIN);
static DigitalOut g_ldr(LDR_PIN);

static Ticker g_ticker1;  //ticker para cuenta ascendente
static Ticker g_ticker2;  //multiplexacion
static Timeout g_to;
static bool volatile gb_toggle1;
static bool volatile gb_toggle2;
static bool volatile gb_ldr_evnt;
static bool volatile gb_ldl_evnt;

static void tick1_isr (){ gb_toggle1 = true;}
static void tick2_isr (){ gb_toggle2 = true;}
static void ldl_isr (){ gb_ldl_evnt = true;}
static void ldr_isr (){ gb_ldr_evnt = true;}

int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  return sseg[code];
} 
  
int main (void) {
  uint8_t contador = 15;
  g_ticker1.attach(tick1_isr,200ms);  //5Hz
  g_ticker2.attach(tick2_isr,1250us); //800Hz
  g_dsl = 0;
  g_dsr = 1;
  g_seg = (1 == g_dsl) ? to_7seg(contador/10) : to_7seg(contador%10);

  for (;;) {
    if(gb_toggle1){
      gb_toggle1 = false;
      g_ldr = 0;
      g_ldl = 0;
      g_seg = (1 == g_dsl) ? to_7seg(contador/10) : to_7seg(contador%10);
      if(contador < 55){
        contador ++;
      }else if(contador == 55){
        contador = 0;
        g_ldl = 1;
      }else{}
      if(contador % 5 == 0 || contador % 10 == 0){
        g_ldr = 1;
      }
    }
    if(gb_toggle2){
      gb_toggle2 = false;
      if(g_dsl == 1){
        g_dsl = 0;
        g_dsr = 1;
      }else if( g_dsr == 1){
        g_dsl = 1;
        g_dsr = 0;
      }else{}
      g_seg = (1 == g_dsl) ? to_7seg(contador/10) : to_7seg(contador%10);
      }
  }// forever
  __disable_irq();
  if(!gb_toggle1 && !gb_toggle2){
    __WFI();
  }
  __enable_irq();
}  // main()


