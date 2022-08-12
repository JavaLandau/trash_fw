#include "jhal_gpio.h"
#include JHAL_GPIO_INCLUDE_NAME

struct _instance_list{  
  void*                         puser_data;
  jhal_type_gpio_input          pfunc_input;
  struct _instance_list*        pnext;
  struct _instance_list*        pprev;
  void*                         pinstance;
};

typedef struct _instance_list instance_list;

static instance_list* plist_top = NULL;

__WEAK uint8_t JHAL_GPIO_INIT(void* pinstance, jhal_gpio_params* pparams)
{
  (void)pinstance;
  (void)pparams;  

  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_GPIO_DEINIT(void* pinstance)
{
  (void)pinstance;
  
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_GPIO_SET(void* pinstance, uint64_t pins, uint8_t value)
{
  (void)pinstance;
  (void)pins;
  (void)value;
  
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_GPIO_GET(void* pinstance, uint64_t pins, uint8_t* pvalue)
{
  (void)pinstance;
  (void)pins;
  (void)pvalue;
  
  return JHAL_RES_NOT_SUPPORTED;
}

uint8_t jhal_gpio_init(void** ppinstance, jhal_gpio_params* pparams)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)       
   if(!ppinstance || *ppinstance || !pparams) 
     return JHAL_RES_INVALID_PARAMS;
#endif      
   instance_list* plist_new = (instance_list*)jhal_malloc(JHAL_GPIO_SIZE_DRV + sizeof(instance_list));
   
   if(plist_new == NULL)  
     return JHAL_RES_ALLOC_ERROR;
          
   plist_new->pfunc_input = pparams->pfunc_input;
   plist_new->puser_data = pparams->puser_data;
   
   uint8_t res = JHAL_GPIO_INIT(plist_new->pinstance, pparams); 
   
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

uint8_t jhal_gpio_deinit(void* pinstance)
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
  
  return JHAL_GPIO_DEINIT(pinstance);
}

uint8_t jhal_gpio_set(void* pinstance, uint64_t pins, uint8_t value)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance && !pins && value > 1) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
  return JHAL_GPIO_SET(pinstance, pins, value);
}

uint8_t jhal_gpio_get(void* pinstance, uint64_t pins, uint8_t* pvalue)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance && !pins && !pvalue) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   
  return JHAL_GPIO_GET(pinstance, pins, pvalue);
}

void jhal_gpio_input_callback(void* pinstance, uint16_t pin, uint8_t value)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
  JHAL_ASSERT(pinstance && value <= 1);  
#endif
  instance_list* plist = plist_top;
  while(plist != NULL)
  {
    if(plist->pinstance == pinstance)
    {
      if(plist->pfunc_input)
        plist->pfunc_input(plist->puser_data, pin, value);
      
      break;
    }
    plist = plist->pnext;
  }    
}