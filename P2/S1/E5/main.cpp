#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static PwmOut g_dsl(DSL_PIN);
static PwmOut g_dsr(DSR_PIN);
static DigitalOut g_ldr(LDR_PIN);
static AnalogIn g_lit(LIT_PIN);

static Ticker g_tick1;
static Ticker g_tick2;
static Ticker g_tick3;
static Timeout g_to;

static bool volatile gb_toggle1; //boolean de contador
static bool volatile gb_toggle2; 
static bool volatile gb_toggle3; //boolean de led
static bool volatile gb_on_evnt;

static void tick1_isr (void) {gb_toggle1=true;}   //ticker de contador
static void tick2_isr (void) {gb_toggle2=true;}   //ticker para PWM
static void tick3_isr (void) {gb_toggle3=true;}   //ticker de led
static void to_isr (void) {gb_on_evnt=true;}
static void to_ldr (void) {g_ldr=1;}

int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
  return sseg[code];
} 
int main (void) {
  g_tick1.attach(tick1_isr, 330ms); //frecuencia de contador
  g_tick2.attach(tick2_isr,40us);  //frecuencia de PWM
  g_tick3.attach(tick3_isr,10ms);  //frecuencia de led
  g_dsr.period_us(40);
  g_dsl.period_us(40);
  int width_us = 0;
  uint8_t lit = g_lit.read_u16();
  int contador=0;
  g_dsr=0;
  g_dsl=1;
  g_ldr=0;
  to_isr();

  for (;;) {
    if( gb_toggle1 ){
      gb_toggle1 = false;
      contador = lit*100/65535 ;
    } //gb_toggle1
    if( gb_toggle2 ){ 
      gb_toggle2 = false;
      
      if(gb_on_evnt){
        gb_on_evnt=false;
        g_dsl = 0;
        g_seg = to_7seg(contador%10);  //valor unidad
        g_dsr = 1;
        g_dsr.pulsewidth_ms(width_us);
      }
      else{
        gb_on_evnt=true;
        g_dsr=0;
        g_seg=to_7seg(contador/10);  //valor decimal
        g_dsl=1;
        g_dsl.pulsewidth_ms(width_us);
      }
     } //gb_toggle2
    
    if(gb_toggle3){
      gb_toggle3=false;
      g_to.attach(to_ldr,(std::chrono::microseconds)contador);
    } //gb_toggle3
    
    __disable_irq();
    if(!gb_toggle1 && !gb_on_evnt){
      __WFI();
    }
    __enable_irq();
    }// forever
 
}  // main()


