#include "stm32f4xx_hal.h"
#include "jhal_tick.h"

#define STM32F4XX_FREQ          180000000

#define    DWT_CYCCNT    *(volatile uint32_t *)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t *)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t *)0xE000EDFC

static uint32_t DWT_Get(void)
{
    return DWT_CYCCNT;
}

uint32_t env_stm32f4xx_hal_tick_init(void)
{
  if( !(DWT_CONTROL & 1) )
  {
    SCB_DEMCR |= 0x01000000;
    DWT_CONTROL |= 1;
  }      
  
  return JHAL_RES_NO_ERRORS;
}

uint32_t env_stm32f4xx_hal_tick(uint32_t delay)
{
    int32_t tp = DWT_Get() + delay * (STM32F4XX_FREQ / 1000000);
    while((((int32_t)DWT_Get() - tp) < 0));
    
    return JHAL_RES_NO_ERRORS;
}
