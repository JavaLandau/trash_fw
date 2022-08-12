#ifndef __JHAL_TIM__
#define __JHAL_TIM__

#ifdef __cplusplus
extern "C" {
#endif

#include "jhal_environment.h"

typedef void (*jhal_type_tim_base_period_ellapsed)(void*);
  
#define JHAL_TIM_CLOCK_SOURCE_INTERNAL          12U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_1        144U  
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_2        32U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_3        225U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_4        113U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_5        89U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_6        94U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_7        33U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_8        117U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_9        11U
#define JHAL_TIM_CLOCK_SOURCE_EXTERNAL_10       100U
  
typedef enum {
  JHAL_TIM_BASE_TYPE_COUNTER_DOWN  =  31U,
  JHAL_TIM_BASE_TYPE_COUNTER_UP    =  74U
} jhal_tim_base_type_counter;
  
typedef struct {
  uint8_t                               num_module;
  uint8_t                               clock_source;
  uint32_t                              prescaler; 
  uint32_t                              clock_prescaler;
  uint32_t                              period;
  uint32_t                              preload_value;
  uint32_t                              counter;
  jhal_tim_base_type_counter            type_counter;
  
  jhal_type_tim_base_period_ellapsed    pfunc_period_ellapsed;
  void*                                 plib_data;
  void*                                 puser_data;
} jhal_tim_base_params;


uint8_t jhal_tim_base_init(void** ppinstance, jhal_tim_base_params* pparams);
uint8_t jhal_tim_base_deinit(void* pinstance);
uint8_t jhal_tim_base_start(void* pinstance);
uint8_t jhal_tim_base_start_it(void* pinstance);
uint8_t jhal_tim_base_start_dma(void* pinstance, uint8_t* pData, uint16_t size, void* pinstance_dma);
uint8_t jhal_tim_base_stop(void* pinstance);
uint8_t jhal_tim_base_stop_it(void* pinstance);
uint8_t jhal_tim_base_stop_dma(void* pinstance, void* pinstance_dma);

void jhal_tim_base_period_ellapsed_callback(void* pinstance);


#ifdef __cplusplus
}
#endif

#endif