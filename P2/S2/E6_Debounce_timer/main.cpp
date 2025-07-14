#include "mbed.h"
#include "pinout.h"
#include "to_7seg.h"

// seven segment display anodes
// when in a int8_t, they are 0b-GFEDCBA
static BusOut         g_seven_seg(SGA_PIN, SGB_PIN, SGC_PIN, SGD_PIN,
                                  SGE_PIN, SGF_PIN, SGG_PIN);

// display cathodes
static DigitalOut     g_dsl(DSL_PIN);
static DigitalOut     g_dsr(DSR_PIN);

// mux stuff
static Ticker         g_mux_tick;
static bool volatile  gb_mux_evnt;

static void mux_isr (void) {
  gb_mux_evnt = true;
}

// switch
static InterruptIn    g_swr(SWR_PIN);

// switch management
static Timer          g_swr_tmr;
static bool volatile  gb_swr_fall_evnt;
static bool volatile  gb_swr_rise_evnt;

static void swr_fall_isr (void) {
  gb_swr_fall_evnt = true;
}

static void swr_rise_isr (void) {
  gb_swr_rise_evnt = true;
}

int main (void) {
  uint8_t cnt = 0;                        // 0 to 99
  bool    b_right = false;
  bool    b_swr_state = false;

  g_swr.mode(PullUp);
  g_mux_tick.attach(mux_isr, 4ms);        // 250 Hz
  g_swr.fall(swr_fall_isr);
  g_swr.rise(swr_rise_isr);
  g_swr_tmr.start();

  g_seven_seg = 0;
  g_dsr = b_right;
  g_dsl = !b_right;
  g_seven_seg = to_7seg(b_right ? cnt % 10 : cnt / 10);

  for (;;) {
    if (gb_mux_evnt) {
      gb_mux_evnt = false;
      b_right = !b_right;
      g_seven_seg = 0;
      g_dsr = b_right;
      g_dsl = !b_right;
      g_seven_seg = to_7seg(b_right ? cnt % 10 : cnt / 10);
    }

    if (gb_swr_fall_evnt) {
      gb_swr_fall_evnt = false;
      if ((!b_swr_state) && (g_swr_tmr.elapsed_time() > 10ms)) {
        b_swr_state = true;
        cnt += ((cnt >= 99) ? -cnt : 1);
      }
      g_swr_tmr.reset();
    }

    if (gb_swr_rise_evnt) {
      gb_swr_rise_evnt = false;
      if (b_swr_state && (g_swr_tmr.elapsed_time() > 10ms)) {
        b_swr_state = false;
      }
      g_swr_tmr.reset();
    }

    __disable_irq();
    if (!gb_mux_evnt && !gb_swr_fall_evnt && !gb_swr_rise_evnt) {
      __WFI();
    }
    __enable_irq();
  } // forever
} // main()
