#include "DeviceLCD1602.h"
#include "DeviceLCD1602_GPIO.h"
#include "jhal_gpio.h"
#include "jhal_tick.h"

#define TIME_FOR_DELAY          1

uint8_t LCD1602Init_GPIO(void** ppInstance, void* pParams)
{
  return jhal_gpio_init(ppInstance, (jhal_gpio_params*)pParams);
}

uint8_t LCD1602EnableDisable_GPIO(void* pInstance, void* pParams, uint8_t valueE)
{
  DeviceLCD1602_GPIOParams* pGPioParams = (DeviceLCD1602_GPIOParams*)pParams;
  
  jhal_gpio_set(pInstance, pGPioParams->pinE, valueE);
  
  return JHAL_RES_NO_ERRORS;
}

uint8_t LCD1602ReadWrite_GPIO(void* pInstance, void* pParams, uint8_t* pData, uint8_t valueRS, uint8_t valueRW)
{
  DeviceLCD1602_GPIOParams* pGPioParams = (DeviceLCD1602_GPIOParams*)pParams;
  
  jhal_gpio_set(pInstance, pGPioParams->pinRS, valueRS);
  jhal_gpio_set(pInstance, pGPioParams->pinRW, valueRW);
    
  jhal_tick(TIME_FOR_DELAY);
  if(valueRW)
  {    
    *pData = 0;
    uint8_t ReadBit;
    for(uint8_t i = 0; i < LCD1602_LEN_DATA; i++)
    {      
      jhal_gpio_get(pInstance, pGPioParams->pinsData[i], &ReadBit);
      
      if(ReadBit)
        *pData |=(1 << i);
    }
  }else
  {
    for(uint8_t i = 0; i < LCD1602_LEN_DATA; i++)
      jhal_gpio_set(pInstance, pGPioParams->pinsData[i], ((*pData) >> i) & 0x1);
  }
  
  return JHAL_RES_NO_ERRORS;
}

uint8_t LCD1602Deinit_GPIO(void* pInstance)
{
  (void)pInstance;
  return JHAL_RES_NOT_SUPPORTED;
}
