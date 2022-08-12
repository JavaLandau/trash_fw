#ifndef __ENV_STM32F4XX_HAL_GPIO__
#define __ENV_STM32F4XX_HAL_GPIO__

uint32_t env_stm32f4xx_hal_gpio_size_drv(void);
uint8_t env_stm32f4xx_hal_gpio_init(void* pInstance, jhal_gpio_params* pParams);
uint8_t env_stm32f4xx_hal_gpio_set(void* pinstance, uint64_t pins, uint8_t value);
uint8_t env_stm32f4xx_hal_gpio_get(void* pinstance, uint64_t pins, uint8_t* pValue);

#endif