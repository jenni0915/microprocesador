#include "mbed.h"
#include "pinout.h"
#include "sw_tick_serial.h"

static BusOut myDisplay(SGA_PIN,SGB_PIN,SGC_PIN,SGD_PIN,SGE_PIN,SGF_PIN,SGG_PIN);
static DigitalOut  dsr (DSR_PIN);
int contador = 0;
static int8_t numeros[10]={0x3F,0x06,0x5B,0x4F,0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
 
int main (void) {
  
  sw_tick_serial_init();
  
  for (;;) {
    if(gb_tick_1s_evnt){
      gb_tick_1s_evnt = false;
      while(contador<=10){
        contador++;
        myDisplay=numeros[10];
      }
      contador=0;
  } // forever
} // main(void)

