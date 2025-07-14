#ifndef TO_7SEG_H
#define TO_7SEG_H

#include "mbed.h"

// converts to seven segment (active high, GFEDCBA)
// input codes from 0 to 15 give simbols:
//  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, -, r, n, E, F}
//  other inputs give a blank display
#ifdef __cplusplus
extern "C" {
#endif

int8_t to_7seg(uint8_t code);

#ifdef __cplusplus
}
#endif

#endif  // TO_7SEG_H
