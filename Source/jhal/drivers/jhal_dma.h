#ifndef __JHAL_DMA__
#define __JHAL_DMA__

#ifdef __cplusplus
extern "C" {
#endif

#include "jhal_environment.h"

#define  JHAL_DMA_PRIORITY_LOWEST       0U
#define  JHAL_DMA_PRIORITY_LOW          1U
#define  JHAL_DMA_PRIORITY_MIDDLE       2U
#define  JHAL_DMA_PRIORITY_HIGH         3U
#define  JHAL_DMA_PRIORITY_HIGHEST      4U  
  
typedef void (*jhal_type_dma_transfer_complete)(void*, void*);

typedef enum {
  JHAL_DMA_DIRECTION_PERIPH_TO_MEM      = 96U,
  JHAL_DMA_DIRECTION_MEM_TO_PERIPH      = 199U,
  JHAL_DMA_DIRECTION_PERIPH_TO_PERIPH   = 11U,
  JHAL_DMA_DIRECTION_MEM_TO_MEM         = 129U
} jhal_dma_direction;  
  
typedef enum {
  JHAL_DMA_DATA_SIZE_8BIT               = 7U,
  JHAL_DMA_DATA_SIZE_16BIT              = 34U,
  JHAL_DMA_DATA_SIZE_32BIT              = 88U
} jhal_dma_data_size;

typedef enum {
  JHAL_DMA_INCREMENT_TYPE_DISABLE       = 111U,
  JHAL_DMA_INCREMENT_TYPE_ENABLE        = 206U
} jhal_dma_increment_type;

typedef struct {
  uint8_t                               num_module;
  uint8_t                               num_channel;
  uint32_t                              priority;
  jhal_dma_direction                    direction;  
  jhal_dma_data_size                    source_data_size;  
  jhal_dma_data_size                    destination_data_size;
  jhal_dma_increment_type               source_increment_type;
  jhal_dma_increment_type               destination_increment_type;
  
  jhal_type_dma_transfer_complete       pfunc_transfer_complete;
  void*                                 plib_data;
  void*                                 puser_data;
} jhal_dma_params;

uint8_t jhal_dma_init(void** ppinstance, jhal_dma_params* pparams);
uint8_t jhal_dma_deinit(void* pinstance);
uint8_t jhal_dma_start(void* pinstance, uint32_t srcaddress, uint32_t dstaddress, uint32_t size);
uint8_t jhal_dma_stop(void* pinstance);
uint8_t jhal_dma_start_it(void* pinstance, uint32_t srcaddress, uint32_t dstaddress, uint32_t size);
uint8_t jhal_dma_stop_it(void* pinstance);

void jhal_dma_transfer_complete_callback(void* pInstance);

#ifdef __cplusplus
}
#endif

#endif