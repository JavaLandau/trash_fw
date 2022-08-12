#ifndef __JHAL_ENV__
#define __JHAL_ENV__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  
#include <stdio.h>  
#include <stddef.h>
#include "jhal_settings.h"  
    
#define JHAL_VERSION                    "0.0"  
    
#define JHAL_RES_NO_ERRORS              0U  
#define JHAL_RES_INVALID_PARAMS         1U  
#define JHAL_RES_ALLOC_ERROR            2U  
#define JHAL_RES_NOT_SUPPORTED          3U  
#define JHAL_RES_TIMEOUT                4U    
#define JHAL_RES_ERROR                  5U  

#if (JHAL_SIZE_ADD_PARAMS > 0)  
typedef struct {
   uint8_t param_amount;
   uint16_t param_id[JHAL_SIZE_ADD_PARAMS];
   uint32_t param_value[JHAL_SIZE_ADD_PARAMS];
} jhal_add_params;
#endif

typedef struct {
  void*                 puser_data;
  uint32_t              instance_signature;
  void*                 pfuncs_callbacks;
  void*                 pinstance;
} jhal_driver_instance;

#define INSTANCE_SIGNATURE              0x523487AC

void* jhal_malloc(uint32_t size);
void jhal_free(void* pinstance);

jhal_driver_instance* jhal_driver_malloc(uint32_t size_instance, uint32_t size_driver);
void jhal_driver_free(void* pinstance);

#define JHAL_OFFSET(INSTANCE, TYPE, MEMBER)             ((uint32_t)INSTANCE + offsetof(TYPE, MEMBER))

#define JHAL_MEMBER_BY_INSTANCE(INSTANCE, MEMBER)       (((uint32_t)INSTANCE - offsetof(jhal_driver_instance, pinstance) +\
                                                        offsetof(jhal_driver_instance, MEMBER))) 

#define JHAL_GET_SIGNATURE(INSTANCE)                    (*((uint32_t*)JHAL_MEMBER_BY_INSTANCE(INSTANCE, instance_signature)))
#define JHAL_GET_FUNC_CALLBACKS(INSTANCE)               (*((void**)(JHAL_MEMBER_BY_INSTANCE(INSTANCE, pfuncs_callbacks))))
#define JHAL_GET_USERDATA(INSTANCE)                     (*((void**)(JHAL_MEMBER_BY_INSTANCE(INSTANCE, puser_data))))

#define JHAL_CHECK_INSTANCE(INSTANCE)                   (JHAL_GET_SIGNATURE(INSTANCE) == INSTANCE_SIGNATURE)

#define JHAL_CARCASS_FUNC(INSTANCE, TYPE, MEMBER_TYPE, MEMBER)   (*((MEMBER_TYPE*)((uint32_t)JHAL_GET_FUNC_CALLBACKS(INSTANCE) + offsetof(TYPE, MEMBER))))\
                                                                 (INSTANCE, JHAL_GET_USERDATA(INSTANCE)

//#define JHAL_CARCASS_FUNC(INSTANCE, TYPE, MEMBER)       (*((MEMBER)*)((uint32_t)JHAL_GET_FUNC_CALLBACKS(INSTANCE) + offsetof(TYPE, MEMBER)))\
//                                                        (INSTANCE, JHAL_GET_USERDATA(INSTANCE)

#if defined(JHAL_MCU) && defined(JHAL_LIB)
  #if (JHAL_LIB == 1)
    #define JHAL_MCU_ENV                        JHAL_MCU
  #else
    #define JHAL_BUILD_ENV(MCU_TYPE,LIB_TYPE)   MCU_TYPE ## _ ## LIB_TYPE
    #define JHAL_BUILD_ENV2(MCU_TYPE,LIB_TYPE)  JHAL_BUILD_ENV(MCU_TYPE,LIB_TYPE)
    #define JHAL_MCU_ENV                        JHAL_BUILD_ENV2(JHAL_MCU,JHAL_LIB)
  #endif
#else
  #ifndef JHAL_MCU
    #error "No declaration of define JHAL_MCU!"
  #endif
  
  #ifndef JHAL_LIB
    #error "No declaration of define JHAL_LIB!"
  #endif  
#endif
  
#if (JHAL_LIB != 1) 
  #define JHAL_CREATE_FUNCTION(MCU_TYPE,DRV_TYPE,ACTION)                          env_ ## MCU_TYPE ## DRV_TYPE ## ACTION
  #define JHAL_CREATE_INCLUDE(MCU_TYPE,DRV_TYPE)                                  env_ ## MCU_TYPE ## DRV_TYPE
#else
  #define JHAL_CREATE_FUNCTION(MCU_TYPE,DRV_TYPE,ACTION)                          MCU_TYPE ## DRV_TYPE ## ACTION
  #define JHAL_CREATE_INCLUDE(MCU_TYPE,DRV_TYPE)                                  MCU_TYPE ## DRV_TYPE  
#endif
  
#define JHAL_FUNCTION_NAME(MCU_TYPE,DRV_TYPE,ACTION)                              JHAL_CREATE_FUNCTION(MCU_TYPE,DRV_TYPE,ACTION)
#define JHAL_INCLUDE_NAME(MCU_TYPE,DRV_TYPE)                                      JHAL_CREATE_INCLUDE(MCU_TYPE,DRV_TYPE)

#if ( USE_JHAL_ASSERT  == 1)
  #define JHAL_ENV_INCLUDE_NAME_WITHOUT_QUOTES                                    JHAL_INCLUDE_NAME(JHAL_MCU_ENV,.h)
  #define JHAL_ENV_INCLUDE_NAME_WITH_QUOTES(INCLUDE_NAME)                         #INCLUDE_NAME
  #define JHAL_ENV_INCLUDE_NAME_WITH_QUOTES2(INCLUDE_NAME)                        JHAL_ENV_INCLUDE_NAME_WITH_QUOTES(INCLUDE_NAME)
  #define JHAL_ENV_INCLUDE_NAME                                                   JHAL_ENV_INCLUDE_NAME_WITH_QUOTES2(JHAL_SPI_INCLUDE_NAME_WITHOUT_QUOTES)
  
  #define JHAL_ASSERT(CONDITION)                                                  do {if(!(CONDITION))JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_assert)(__FILE__,__LINE__); }while(0)
#else
  #define JHAL_ASSERT(CONDITION)                                                  do {}while(0)
#endif

#define JHAL_INCLUDE_NAME_WITH_QUOTES(INCLUDE_NAME)                               #INCLUDE_NAME
#define JHAL_INCLUDE_NAME_WITH_QUOTES2(INCLUDE_NAME)                              JHAL_INCLUDE_NAME_WITH_QUOTES(INCLUDE_NAME)
 
#define JHAL_TICK_INCLUDE_NAME_WITHOUT_QUOTES                                     JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_tick.h)
#define JHAL_TICK_INCLUDE_NAME                                                    JHAL_INCLUDE_NAME_WITH_QUOTES2(JHAL_TICK_INCLUDE_NAME_WITHOUT_QUOTES)

#define JHAL_TICK(AMOUNT_US)                                                      JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_tick)(AMOUNT_US)
#define JHAL_TICK_INIT                                                            JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_tick_init)
                                             
#define JHAL_SPI_INCLUDE_NAME_WITHOUT_QUOTES                                      JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_spi.h)
#define JHAL_SPI_INCLUDE_NAME                                                     JHAL_INCLUDE_NAME_WITH_QUOTES2(JHAL_SPI_INCLUDE_NAME_WITHOUT_QUOTES)

#define JHAL_SPI_SIZE_DRV                                                         JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_size_drv)()
#define JHAL_SPI_INIT(INSTANCE,PARAMS)                                            JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_init)(INSTANCE,PARAMS)
#define JHAL_SPI_DEINIT(INSTANCE)                                                 JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_deinit)(INSTANCE)
#define JHAL_SPI_TRANSMIT(INSTANCE,TXDATA,SIZE,TIMEOUT)                           JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_transmit)(INSTANCE,TXDATA,SIZE,TIMEOUT)
#define JHAL_SPI_RECEIVE(INSTANCE,RXDATA,SIZE,TIMEOUT)                            JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_receive)(INSTANCE,RXDATA,SIZE,TIMEOUT)
#define JHAL_SPI_TRANSMITRECEIVE(INSTANCE,TXDATA,RXDATA,SIZE,TIMEOUT)             JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_transmitreceive)(INSTANCE,TXDATA,RXDATA,SIZE,TIMEOUT)
#define JHAL_SPI_TRANSMIT_IT(INSTANCE,TXDATA,SIZE)                                JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_transmit_it)(INSTANCE,TXDATA,SIZE)
#define JHAL_SPI_RECEIVE_IT(INSTANCE,RXDATA,SIZE)                                 JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_receive_it)(INSTANCE,RXDATA,SIZE)
#define JHAL_SPI_TRANSMITRECEIVE_IT(INSTANCE,TXDATA,RXDATA,SIZE)                  JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_transmitreceive_it)(INSTANCE,TXDATA,RXDATA,SIZE)
#define JHAL_SPI_TRANSMIT_DMA(INSTANCE,TXDATA,SIZE,INSTANCE_DMA)                  JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_transmit_dma)(INSTANCE,TXDATA,SIZE,INSTANCE_DMA)
#define JHAL_SPI_RECEIVE_DMA(INSTANCE,RXDATA,SIZE,INSTANCE_DMA)                   JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_receive_dma)(INSTANCE,RXDATA,SIZE,INSTANCE_DMA)
#define JHAL_SPI_TRANSMITRECEIVE_DMA(INSTANCE,TXDATA,RXDATA,SIZE,INSTANCE_DMA)    JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_spi,_transmitreceive_dma)(INSTANCE,TXDATA,RXDATA,SIZE,INSTANCE_DMA)


#define JHAL_GPIO_INCLUDE_NAME_WITHOUT_QUOTES                                     JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_gpio.h)
#define JHAL_GPIO_INCLUDE_NAME                                                    JHAL_INCLUDE_NAME_WITH_QUOTES2(JHAL_GPIO_INCLUDE_NAME_WITHOUT_QUOTES)

#define JHAL_GPIO_SIZE_DRV                                                        JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_gpio,_size_drv)()
#define JHAL_GPIO_INIT(INSTANCE,PARAMS)                                           JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_gpio,_init)(INSTANCE,PARAMS)
#define JHAL_GPIO_DEINIT(INSTANCE)                                                JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_gpio,_deinit)(INSTANCE)
#define JHAL_GPIO_SET(INSTANCE,PIN,VALUE)                                         JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_gpio,_set)(INSTANCE,PIN,VALUE)
#define JHAL_GPIO_GET(INSTANCE,PIN,VALUE)                                         JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_gpio,_get)(INSTANCE,PIN,VALUE)

#define JHAL_UART_INCLUDE_NAME_WITHOUT_QUOTES                                     JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_uart.h)
#define JHAL_UART_INCLUDE_NAME                                                    JHAL_INCLUDE_NAME_WITH_QUOTES2(JHAL_UART_INCLUDE_NAME_WITHOUT_QUOTES)

#define JHAL_UART_SIZE_DRV                                                        JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_size_drv)()
#define JHAL_UART_INIT(INSTANCE,PARAMS)                                           JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_init)(INSTANCE,PARAMS)
#define JHAL_UART_DEINIT(INSTANCE)                                                JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_deinit)(INSTANCE)
#define JHAL_UART_TRANSMIT(INSTANCE,TXDATA,SIZE,TIMEOUT)                          JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_transmit)(INSTANCE,TXDATA,SIZE,TIMEOUT)
#define JHAL_UART_RECEIVE(INSTANCE,RXDATA,SIZE,TIMEOUT)                           JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_receive)(INSTANCE,RXDATA,SIZE,TIMEOUT)
#define JHAL_UART_TRANSMITRECEIVE(INSTANCE,TXDATA,RXDATA,SIZE,TIMEOUT)            JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_transmitreceive)(INSTANCE,TXDATA,RXDATA,SIZE,TIMEOUT)
#define JHAL_UART_TRANSMIT_IT(INSTANCE,TXDATA,SIZE)                               JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_transmit_it)(INSTANCE,TXDATA,SIZE)
#define JHAL_UART_RECEIVE_IT(INSTANCE,RXDATA,SIZE)                                JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_receive_it)(INSTANCE,RXDATA,SIZE)
#define JHAL_UART_TRANSMITRECEIVE_IT(INSTANCE,TXDATA,RXDATA,SIZE)                 JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_transmitreceive_it)(INSTANCE,TXDATA,RXDATA,SIZE)
#define JHAL_UART_TRANSMIT_DMA(INSTANCE,TXDATA,SIZE,INSTANCE_DMA)                 JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_transmit_dma)(INSTANCE,TXDATA,SIZE,INSTANCE_DMA)
#define JHAL_UART_RECEIVE_DMA(INSTANCE,RXDATA,SIZE,INSTANCE_DMA)                  JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_receive_dma)(INSTANCE,RXDATA,SIZE,INSTANCE_DMA)
#define JHAL_UART_TRANSMITRECEIVE_DMA(INSTANCE,TXDATA,RXDATA,SIZE,INSTANCE_DMA)   JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_uart,_transmitreceive_dma)(INSTANCE,TXDATA,RXDATA,SIZE,INSTANCE_DMA)


#define JHAL_DMA_INCLUDE_NAME_WITHOUT_QUOTES                                      JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_dma.h)
#define JHAL_DMA_INCLUDE_NAME                                                     JHAL_INCLUDE_NAME_WITH_QUOTES2(JHAL_DMA_INCLUDE_NAME_WITHOUT_QUOTES)

#define JHAL_DMA_SIZE_DRV                                                         JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_size_drv)()
#define JHAL_DMA_INIT(INSTANCE,PARAMS)                                            JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_init)(INSTANCE,PARAMS)
#define JHAL_DMA_DEINIT(INSTANCE)                                                 JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_deinit)(INSTANCE)
#define JHAL_DMA_START(INSTANCE,SRCADDRESS,DSTADDRESS,SIZE)                       JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_start)(INSTANCE,SRCADDRESS,DSTADDRESS,SIZE)
#define JHAL_DMA_STOP(INSTANCE)                                                   JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_stop)(INSTANCE)
#define JHAL_DMA_START_IT(INSTANCE,SRCADDRESS,DSTADDRESS,SIZE)                    JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_start_it)(INSTANCE,SRCADDRESS,DSTADDRESS,SIZE)
#define JHAL_DMA_STOP_IT(INSTANCE)                                                JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_dma,_stop_it)(INSTANCE)


#define JHAL_TIM_BASE_INCLUDE_NAME_WITHOUT_QUOTES                                 JHAL_INCLUDE_NAME(JHAL_MCU_ENV,_tim_base.h)
#define JHAL_TIM_BASE_INCLUDE_NAME                                                JHAL_INCLUDE_NAME_WITH_QUOTES2(JHAL_TIM_BASE_INCLUDE_NAME_WITHOUT_QUOTES)

#define JHAL_TIM_BASE_SIZE_DRV                                                    JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_size_drv)()
#define JHAL_TIM_BASE_INIT(INSTANCE,PARAMS)                                       JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_init)(INSTANCE,PARAMS)
#define JHAL_TIM_BASE_DEINIT(INSTANCE)                                            JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_deinit)(INSTANCE)
#define JHAL_TIM_BASE_START(INSTANCE)                                             JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_start)(INSTANCE)
#define JHAL_TIM_BASE_STOP(INSTANCE)                                              JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_stop)(INSTANCE)
#define JHAL_TIM_BASE_START_IT(INSTANCE)                                          JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_start_it)(INSTANCE)
#define JHAL_TIM_BASE_STOP_IT(INSTANCE)                                           JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_stop_it)(INSTANCE)
#define JHAL_TIM_BASE_START_DMA(INSTANCE,PDATA,SIZE,INSTANCE_DMA)                 JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_start_dma)(INSTANCE,PDATA,SIZE,INSTANCE_DMA)
#define JHAL_TIM_BASE_STOP_DMA(INSTANCE,INSTANCE_DMA)                             JHAL_FUNCTION_NAME(JHAL_MCU_ENV,_tim_base,_stop_dma)(INSTANCE,INSTANCE_DMA)

#ifdef __cplusplus
}
#endif

#endif