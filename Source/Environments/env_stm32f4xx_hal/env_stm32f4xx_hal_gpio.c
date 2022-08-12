#include "stm32f4xx_hal.h"
#include "jhal_gpio.h"

uint32_t env_stm32f4xx_hal_gpio_size_drv(void)
{
  return sizeof(GPIO_TypeDef*);
}

uint8_t env_stm32f4xx_hal_gpio_init(void* pInstance, jhal_gpio_params* pParams)
{
  GPIO_TypeDef** ppGPIODef = (GPIO_TypeDef**)pInstance;
  *ppGPIODef = NULL;
      
  switch(pParams->num_module)
  {
#ifdef GPIOA
    case JHAL_GPIO_BANK_A:
      *ppGPIODef = GPIOA;
    break;
#endif
#ifdef GPIOB
    case JHAL_GPIO_BANK_B:
      *ppGPIODef = GPIOB;      
    break;
#endif
#ifdef GPIOC
    case JHAL_GPIO_BANK_C:
      *ppGPIODef = GPIOC;      
    break;
#endif
#ifdef GPIOD
    case JHAL_GPIO_BANK_D:
      *ppGPIODef = GPIOD;      
    break;
#endif
#ifdef GPIOE
    case JHAL_GPIO_BANK_E:
      *ppGPIODef = GPIOE;      
    break;
#endif
#ifdef GPIOF
    case JHAL_GPIO_BANK_F:
      *ppGPIODef = GPIOF;      
    break;
#endif
#ifdef GPIOG
    case JHAL_GPIO_BANK_G:
      *ppGPIODef = GPIOG;      
    break;
#endif
#ifdef GPIOH
    case JHAL_GPIO_BANK_H:
      *ppGPIODef = GPIOH;      
    break;
#endif
  }
  
  if(*ppGPIODef == NULL)
    return JHAL_RES_INVALID_PARAMS;
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = pParams->pins;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  
  switch(pParams->pull_type)
  {
    case JHAL_GPIO_PULL_TYPE_NOT_PULL:
      GPIO_InitStruct.Pull = GPIO_NOPULL;
    break;
    case JHAL_GPIO_PULL_TYPE_PULLDOWN:
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    break;
    case JHAL_GPIO_PULL_TYPE_PULLUP:
      GPIO_InitStruct.Pull = GPIO_PULLUP;
    break;      
  }
  
  if(pParams->mode == JHAL_GPIO_MODE_INPUT)
  {  
    switch(pParams->int_type)
    {
      case JHAL_GPIO_INT_TYPE_NOT_SET:
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      break;  
      case JHAL_GPIO_INT_TYPE_RISING_EDGE:
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
      break;  
      case JHAL_GPIO_INT_TYPE_FALLING_EDGE:
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
      break;  
      case JHAL_GPIO_INT_TYPE_BOTH_EDGES:
        GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
      break;  
      default:
        return JHAL_RES_NOT_SUPPORTED;        
    }
  } else if(pParams->mode == JHAL_GPIO_MODE_OUTPUT)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  } else if(pParams->mode == JHAL_GPIO_MODE_OUTPUT_OPENDRAIN)
  {
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  }
  
  HAL_GPIO_Init(*ppGPIODef, &GPIO_InitStruct);
  
  return JHAL_RES_NO_ERRORS;
}

uint8_t env_stm32f4xx_hal_gpio_set(void* pinstance, uint64_t pins, uint8_t value)
{
  if(pins > GPIO_PIN_15)
    return JHAL_RES_NOT_SUPPORTED;
  
  if(value == 0)
    HAL_GPIO_WritePin(*((GPIO_TypeDef**)pinstance), pins, GPIO_PIN_RESET);
  else
    HAL_GPIO_WritePin(*((GPIO_TypeDef**)pinstance), pins, GPIO_PIN_SET);
  
  return JHAL_RES_NO_ERRORS;
}

uint8_t env_stm32f4xx_hal_gpio_get(void* pinstance, uint64_t pins, uint8_t* pValue)
{
  if(pins > GPIO_PIN_15)
    return JHAL_RES_NOT_SUPPORTED;
  
  GPIO_PinState PinState = HAL_GPIO_ReadPin(*((GPIO_TypeDef**)pinstance), pins);
  if(PinState == GPIO_PIN_RESET)
    *pValue = 0;
  else
    *pValue = 1;        
   
  return JHAL_RES_NO_ERRORS;
}
