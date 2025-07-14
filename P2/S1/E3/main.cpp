#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static AnalogIn  g_lit(LIT_PIN);
static Ticker g_tick1; 
static Ticker g_tick2;

static bool volatile gb_toggle1;
static bool volatile gb_toggle2;

static void tick1_isr (void) {gb_toggle1=true;} 
static void tick2_isr (void) {gb_toggle2=true;} 

int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  return sseg[code];
  
} 
int main (void) {
  g_tick1.attach(tick1_isr, 330ms); //frecuencia de contador
  g_tick2.attach(tick2_isr,4ms);  //frecuencia de que muestre al mismo tiempo
  
  float valor=0;
  g_dsr=0;
  g_dsl=1;
  for (;;) {
    if( gb_toggle1 ){
      gb_toggle1 = false;
      valor=g_lit.read_u16()*100/65535;
    }
    if( gb_toggle2 ){ 
      gb_toggle2 = false;
      if(g_dsr == 1){
        g_dsl = 0;
        g_seg = to_7seg((int)valor%10);  //valor unidad
        g_dsr = 1;
      }
      else{
        g_dsr=0;
        g_seg=to_7seg((int)valor/10);  //valor decimal
        g_dsl=1;
      }
     }
     __disable_irq();
     if(!gb_toggle1 && !gb_toggle2){
      __WFI();
     }
     __enable_irq();
    }// forever
 
}  // main()


