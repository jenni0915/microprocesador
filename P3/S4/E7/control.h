#ifndef CONTROL_H
# define CONTGROL_H

#include "mbed.h"

// messages
  // input messages
extern bool volatile gb_ctr_can_sleep;
extern bool gb_changeMode_msg;
  
  
// the FSM
void ctr_fsm(void);


// initialize the FSM. The input parameters are pointers to the mbed
void ctr_init();

#endif  // CONTROL_H

