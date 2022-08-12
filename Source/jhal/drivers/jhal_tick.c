#include "jhal_tick.h"
#include JHAL_TICK_INCLUDE_NAME

__WEAK uint32_t JHAL_TICK(uint32_t amount_us)
{
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint32_t JHAL_TICK_INIT(void)
{
  return JHAL_RES_NOT_SUPPORTED;
}

uint32_t jhal_tick(uint32_t amount_us)
{
  return JHAL_TICK(amount_us);
}

uint32_t jhal_tick_init(void)
{
  return JHAL_TICK_INIT();
}