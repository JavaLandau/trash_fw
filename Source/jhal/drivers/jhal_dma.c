#include "jhal_dma.h"
#include JHAL_DMA_INCLUDE_NAME

typedef struct {
  jhal_type_dma_transfer_complete     pfunc_transfer_complete;
} dma_callback_instance;

__WEAK uint8_t JHAL_DMA_INIT(void* pinstance, jhal_dma_params* pparams)
{
  (void)pinstance;
  (void)pparams;  

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_DMA_DEINIT(void* pinstance)
{
  (void)pinstance;
  
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_DMA_START(void* pinstance, uint32_t srcaddress, uint32_t dstaddress, uint32_t size)
{
  (void)pinstance;
  (void)srcaddress;
  (void)dstaddress;
  (void)size;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_DMA_STOP(void* pinstance)
{
  (void)pinstance;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_DMA_START_IT(void* pinstance, uint32_t srcaddress, uint32_t dstaddress, uint32_t size)
{
  (void)pinstance;
  (void)srcaddress;
  (void)dstaddress;
  (void)size;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_DMA_STOP_IT(void* pinstance)
{
  (void)pinstance;

  return JHAL_RES_NOT_SUPPORTED;
}

uint8_t jhal_dma_init(void** ppinstance, jhal_dma_params* pparams)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)       
   if(!ppinstance || *ppinstance || !pparams) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   jhal_driver_instance* pnew_instance = jhal_driver_malloc(JHAL_DMA_SIZE_DRV, sizeof(dma_callback_instance));
     
   if(pnew_instance == NULL)  
     return JHAL_RES_ALLOC_ERROR;
   
   ((dma_callback_instance*)pnew_instance->pfuncs_callbacks)->pfunc_transfer_complete = pparams->pfunc_transfer_complete;
   pnew_instance->puser_data = pparams->puser_data;
   
   uint8_t res = JHAL_DMA_INIT(pnew_instance->pinstance, pparams); 
   
   if(res == JHAL_RES_NO_ERRORS)       
     *ppinstance = pnew_instance->pinstance;
   else 
   {
     *ppinstance = NULL;
     jhal_driver_free(pnew_instance->pinstance);
   }  
   
   return res;
}

uint8_t jhal_dma_deinit(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif        
  jhal_driver_free(pinstance); 

  return JHAL_DMA_DEINIT(pinstance);
}

uint8_t jhal_dma_start(void* pinstance, uint32_t srcaddress, uint32_t dstaddress, uint32_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !srcaddress || !dstaddress || !size) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_DMA_START(pinstance, srcaddress, dstaddress, size);
}

uint8_t jhal_dma_stop(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_DMA_STOP(pinstance);
}

uint8_t jhal_dma_start_it(void* pinstance, uint32_t srcaddress, uint32_t dstaddress, uint32_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !srcaddress || !dstaddress || !size) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_DMA_START_IT(pinstance, srcaddress, dstaddress, size);
}

uint8_t jhal_dma_stop_it(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_DMA_STOP_IT(pinstance);
}

void jhal_dma_transfer_complete_callback(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
  JHAL_ASSERT(pInstance);  
#endif
  
  if(JHAL_CHECK_INSTANCE(pinstance))
      JHAL_CARCASS_FUNC(pinstance, dma_callback_instance, jhal_type_dma_transfer_complete, pfunc_transfer_complete));
  else {
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
    JHAL_ASSERT(pInstance);  
#endif
  }  
}