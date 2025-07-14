#include "mbed.h"
#include "pinout.h"
#include "sw_tick_serial.h"

static BusOut myDisplay(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static BusOut myLeds(LDR_PIN,LDM_PIN, LDL_PIN);
int contador = 0;
static int8_t numeros[10]={0x3F,0x06,0x5B,0x4F,0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int8_t to_7seg(uint8_t code){
  if(code<10){return numeros[code]}
  else
      return numeros[0];
}

int main (void) {
  
  sw_tick_serial_init();
  myLeds=1;
  for (;;) {
    
    if(gb_tick_1s_evnt){
      gb_tick_1s_evnt = false;
      contador++;
      myDisplay=to_7seg(contador);
      }
    }//if (gb_tick_1s_evnt)
    if(gb_tick_100ms_evnt){
      gb_tick_100ms_evnt=false;
      myLeds=((4==myLeds)?1:(myLeds<<1));
    }//if(gb_tick_10ms_evnt)

  } // forever
} // main(void)

