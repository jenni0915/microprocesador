#include "to_7seg.h"

int8_t to_7seg(int8_t code){
  if(code>=0 && code<=15){   
     return sseg[code];
  }else
    return 0;
} 