#include "jhal_tim_base.h"
#include JHAL_TIM_BASE_INCLUDE_NAME

struct _instance_list{
  void*                               puser_data;  
  jhal_type_tim_base_period_ellapsed  pfunc_period_ellapsed;
  struct _instance_list*              pnext;
  struct _instance_list*              pprev;
  void*                               pinstance;  
};

typedef struct _instance_list instance_list;

static instance_list* plist_top = NULL;

__WEAK uint8_t JHAL_TIM_BASE_INIT(void* pinstance, jhal_tim_base_params* pparams)
{
  (void)pinstance;
  (void)pparams;  

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_DEINIT(void* pinstance)
{
  (void)pinstance;
  
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_START(void* pinstance)
{
  (void)pinstance;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_START_IT(void* pinstance)
{
  (void)pinstance;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_START_DMA(void* pinstance, uint8_t* pdata, uint16_t size, void* pinstance_dma)
{
  (void)pinstance;
  (void)pdata;
  (void)size;
  (void)pinstance_dma;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_STOP(void* pinstance)
{
  (void)pinstance;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_STOP_IT(void* pinstance)
{
  (void)pinstance;

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_TIM_BASE_STOP_DMA(void* pinstance, void* pinstance_dma)
{
  (void)pinstance;
  (void)pinstance_dma;

  return JHAL_RES_NOT_SUPPORTED;
}

uint8_t jhal_tim_base_init(void** ppinstance, jhal_tim_base_params* pparams)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)       
   if(!ppinstance || *ppinstance || !pparams) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   instance_list* plist_new = (instance_list*)jhal_malloc(JHAL_TIM_BASE_SIZE_DRV + sizeof(instance_list));
   
   if(plist_new == NULL)  
     return JHAL_RES_ALLOC_ERROR;
   
   plist_new->pfunc_period_ellapsed = pparams->pfunc_period_ellapsed;
   plist_new->puser_data = pparams->puser_data;
   
   uint8_t res = JHAL_TIM_BASE_INIT(plist_new->pinstance, pparams); 
   
   if(res == JHAL_RES_NO_ERRORS)
   {
     JHAL_DRV_ITEM_ADD(plist_new);
       
     *ppinstance = plist_new->pinstance;
   } else 
   {
     *ppinstance = NULL;
     jhal_free(plist_new);
   } 
   
   return res;
}

uint8_t jhal_tim_base_deinit(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
  instance_list* plist = plist_top;
  while(plist != NULL)
  {
    if(plist->pinstance == pinstance)
      break;

    plist = plist->pnext;
  }
  
  if(plist == NULL)
    return JHAL_RES_INVALID_PARAMS;
  
  JHAL_DRV_ITEM_DELETE(plist);
      
  jhal_free(pinstance);  
  
  return JHAL_TIM_BASE_DEINIT(pinstance);
}

uint8_t jhal_tim_base_start(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_TIM_BASE_START(pinstance);
}

uint8_t jhal_tim_base_start_it(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_TIM_BASE_START_IT(pinstance);
}

uint8_t jhal_tim_base_start_dma(void* pinstance, uint8_t* pData, uint16_t size, void* pinstance_dma)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !pData || !size || !pinstance_dma) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_TIM_BASE_START_DMA(pinstance, pData, size, pinstance_dma);
}

uint8_t jhal_tim_base_stop(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_TIM_BASE_STOP(pinstance);
}

uint8_t jhal_tim_base_stop_it(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_TIM_BASE_STOP_IT(pinstance);
}

uint8_t jhal_tim_base_stop_dma(void* pinstance, void* pinstance_dma)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !pinstance_dma) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
   return JHAL_TIM_BASE_STOP_DMA(pinstance, pinstance_dma);
}

void jhal_tim_base_period_ellapsed_callback(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
  JHAL_ASSERT(pinstance);  
#endif
  instance_list* plist = plist_top;
  while(plist != NULL)
  {
    if(plist->pinstance == pinstance)
    {
      if(plist->pfunc_period_ellapsed)
        plist->pfunc_period_ellapsed(plist->puser_data);
      
      break;
    }
    plist = plist->pnext;
  }    
}