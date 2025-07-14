#include "mbed.h"
#include "pinout.h"

static BusOut g_seg(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut g_dsl(DSL_PIN);
static DigitalOut g_dsr(DSR_PIN);
static InterruptIn g_ech(ECH_PIN);
static DigitalOut g_trg(TRG_PIN);

static Ticker g_tick; //ticker para multiplexacion del display
static Timeout g_trg_to;  //timeout para bajar a cero trg
static Ticker g_trg_tick; //ticker para po ner la senal de trg
static Timer g_ech_t; //timer para medir anchura del echo

static bool volatile gb_tick;     
static bool volatile gb_toggle;  //booleano de ticker 
static bool volatile gb_ech_fall_evnt;
static bool volatile gb_ech_rise_evnt;

static void to_isr (void){
  g_trg = 0;
}

static void trg_tick_isr (void) { 
  gb_toggle = true;
}

static void tick_isr (void){
  gb_tick = true;
}
static void gb_ech_fall_isr (void){
  gb_ech_fall_evnt = true;
}
static void gb_ech_rise_isr (void){
  gb_ech_rise_evnt = true;
}
int8_t to_7seg(uint8_t code){
  static int8_t const sseg[] = {0x3F,0x06,0x5B,0x4F,0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x40, 0x50, 0x54, 0x79, 0x69};
  return sseg[code];
} 

int main (void) {
  std::chrono::microseconds duration;
  bool b_ech_state = false;
  bool b_right = false;
  uint8_t cnt = 0;
  g_trg = 0;
  
  g_trg_tick.attach(trg_tick_isr,100ms);
  g_tick.attach(tick_isr,4ms);
  g_ech.mode(PullUp);
  g_ech.rise(gb_ech_rise_isr);
  g_ech.fall(gb_ech_fall_isr);
  g_dsr = b_right;
  g_dsl = !b_right;
  g_seg = to_7seg(b_right ? cnt %10 : cnt / 10);

  for (;;) {
    if( gb_toggle ){
      gb_toggle = false;
      g_trg = 1;
      g_trg_to.attach(to_isr,1ms);  //bajar trigger 
 
    }
    if( gb_tick ){
      gb_tick = false;
      b_right = !b_right;
      g_dsr = b_right;
      g_dsl = !b_right;
      cnt = (duration.count()>99 ? 12 : duration.count());
      g_seg = to_7seg(b_right ? cnt %10 : cnt / 10);
    }
    
    if(gb_ech_rise_evnt){
      gb_ech_rise_evnt=false;
      g_ech_t.reset();
      g_ech_t.start();
    }
    if(gb_ech_fall_evnt){
      gb_ech_fall_evnt=false;
      g_ech_t.stop();
      duration = g_ech_t.elapsed_time()/58;
      g_ech_t.reset();
     }
    
    __disable_irq();
    if (!gb_toggle && !gb_tick && !gb_ech_rise_evnt && !gb_ech_fall_evnt) {
      __WFI();
    }
    __enable_irq();
    }// forever
 
}  // main()


