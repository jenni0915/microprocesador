#include "to_7seg.h"

// converts to seven segment (active high, GFEDCBA)
// input codes from 0 to 15 give simbols:
//  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, -, r, n, E, F}
//  other inputs give a blank display
int8_t to_7seg (uint8_t code) {
  // complete the code for this function  ++++++++++++++++++++++++++++++
  int8_t const sseg[16] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07, 
    0x7F, 0x6F, 0x77, 0x40,
    0x50, 0x54, 0x79, 0x71
  };
  
  return (code > 0x0f) ? 0 : sseg[code];
  //  ------------------------------------------------------------------
}
