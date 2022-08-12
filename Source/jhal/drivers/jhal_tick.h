#ifndef __JHAL_TICK__
#define __JHAL_TICK__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  
#include "jhal_environment.h"  
  
uint32_t jhal_tick(uint32_t amount_us);

#ifdef __cplusplus
}
#endif

#endif