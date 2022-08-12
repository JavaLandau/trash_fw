#ifndef __JHAL_GPIO__
#define __JHAL_GPIO__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "jhal_environment.h"  
  
#define JHAL_GPIO_BANK_A                    1
#define JHAL_GPIO_BANK_B                    2  
#define JHAL_GPIO_BANK_C                    3
#define JHAL_GPIO_BANK_D                    4
#define JHAL_GPIO_BANK_E                    5  
#define JHAL_GPIO_BANK_F                    6
#define JHAL_GPIO_BANK_G                    7
#define JHAL_GPIO_BANK_H                    8  
#define JHAL_GPIO_BANK_I                    9  
#define JHAL_GPIO_BANK_J                    10  
#define JHAL_GPIO_BANK_K                    11
  
#define JHAL_PIN_NUM(NUM)                   (((uint64_t)1)<<(NUM))
#define JHAL_PIN_RANGE(FROM,TO)             ((JHAL_PIN_NUM(TO) - JHAL_PIN_NUM(FROM - 1)) << 1)

#define JHAL_GPIO_VALUE(NUM)                JHAL_PIN_NUM(NUM)
#define JHAL_GPIO_VALUES_RANGE(FROM,TO)     JHAL_PIN_RANGE(FROM,TO)
  
typedef void (*jhal_type_gpio_input)(void*, uint16_t pin, uint8_t value);
  
typedef enum {  
  JHAL_GPIO_MODE_INPUT                  = 99U,
  JHAL_GPIO_MODE_OUTPUT                 = 13U,
  JHAL_GPIO_MODE_OUTPUT_OPENDRAIN       = 14U
} jhal_gpio_mode;

typedef enum {
  JHAL_GPIO_PULL_TYPE_NOT_PULL          = 114U,
  JHAL_GPIO_PULL_TYPE_PULLDOWN          = 56U,
  JHAL_GPIO_PULL_TYPE_PULLUP            = 10U
} jhal_gpio_pull_type;

typedef enum {
  JHAL_GPIO_INT_TYPE_NOT_SET            = 74U,
  JHAL_GPIO_INT_TYPE_LOW_LEVEL          = 98U,
  JHAL_GPIO_INT_TYPE_HIGH_LEVEL         = 135U,
  JHAL_GPIO_INT_TYPE_RISING_EDGE        = 129U,
  JHAL_GPIO_INT_TYPE_FALLING_EDGE       = 108U,
  JHAL_GPIO_INT_TYPE_BOTH_EDGES         = 244U
} jhal_gpio_int_type;

typedef struct {
  uint8_t                       num_module;
  jhal_gpio_mode                mode;
  jhal_gpio_int_type            int_type;
  jhal_gpio_pull_type           pull_type;
  uint64_t                      pins;
  
  jhal_type_gpio_input          pfunc_input;
  void*                         plib_data;
  void*                         puser_data;
} jhal_gpio_params;

uint8_t jhal_gpio_init(void** ppinstance, jhal_gpio_params* pparams);
uint8_t jhal_gpio_deinit(void* pinstance);
uint8_t jhal_gpio_set(void* pinstance, uint64_t pins, uint8_t value);
uint8_t jhal_gpio_get(void* pinstance, uint64_t pins, uint8_t* pValue);

void jhal_gpio_input_callback(void* pinstance, uint16_t pin, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif