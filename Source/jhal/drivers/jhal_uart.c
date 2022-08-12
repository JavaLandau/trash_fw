#include "jhal_uart.h"
#include JHAL_UART_INCLUDE_NAME

struct _instance_list{
  void*                          puser_data;
  jhal_type_uart_tx_complete     pfunc_tx_complete;
  jhal_type_uart_rx_complete     pfunc_rx_complete;
  jhal_type_uart_txrx_complete   pfunc_txrx_complete;
  struct _instance_list*         pnext;
  struct _instance_list*         pprev;
  void*                          pinstance;    
};

typedef struct _instance_list instance_list;

static instance_list* plist_top = NULL;

__WEAK uint8_t JHAL_UART_INIT(void* pinstance, jhal_uart_params* pparams)
{
  (void)pinstance;
  (void)pparams;  
  
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_UART_DEINIT(void* pinstance)
{
  (void)pinstance;
  
  return JHAL_RES_NOT_SUPPORTED;
}

__WEAK uint8_t JHAL_UART_TRANSMIT(void* pinstance, uint8_t* ptxdata, uint16_t size, uint32_t timeout)
{
  (void)pinstance;
  (void)ptxdata;
  (void)size;
  (void)timeout;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_RECEIVE(void* pinstance, uint8_t* prxdata, uint16_t size, uint32_t timeout)
{
  (void)pinstance;
  (void)prxdata;
  (void)size;
  (void)timeout;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_TRANSMITRECEIVE(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, uint32_t timeout)
{
  (void)pinstance;
  (void)ptxdata;
  (void)prxdata;
  (void)size;
  (void)timeout;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_TRANSMIT_IT(void* pinstance, uint8_t* ptxdata, uint16_t size)
{
  (void)pinstance;
  (void)ptxdata;
  (void)size;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_RECEIVE_IT(void* pinstance, uint8_t* prxdata, uint16_t size)
{
  (void)pinstance;
  (void)prxdata;
  (void)size;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_TRANSMITRECEIVE_IT(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size)
{
  (void)pinstance;
  (void)ptxdata;
  (void)prxdata;
  (void)size;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_TRANSMIT_DMA(void* pinstance, uint8_t* ptxdata, uint16_t size, void* pinstance_dma)
{
  (void)pinstance;
  (void)ptxdata;
  (void)size;
  (void)pinstance_dma;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_RECEIVE_DMA(void* pinstance, uint8_t* prxdata, uint16_t size, void* pinstance_dma)
{
  (void)pinstance;
  (void)prxdata;
  (void)size;
  (void)pinstance_dma;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

__WEAK uint8_t JHAL_UART_TRANSMITRECEIVE_DMA(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, void* pinstance_dma)
{
  (void)pinstance;
  (void)ptxdata;
  (void)prxdata;
  (void)size;
  (void)pinstance_dma;
  
  return JHAL_RES_NOT_SUPPORTED;  
}

uint8_t jhal_uart_init(void** ppinstance, jhal_uart_params* pparams)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)       
   if(!ppinstance || *ppinstance || !pparams) 
     return JHAL_RES_INVALID_PARAMS;
#endif
   instance_list* plist_new = (instance_list*)jhal_malloc(JHAL_UART_SIZE_DRV + sizeof(instance_list));
   
   if(plist_new == NULL)  
     return JHAL_RES_ALLOC_ERROR;
   
   plist_new->pfunc_tx_complete = pparams->pfunc_tx_complete;
   plist_new->pfunc_rx_complete = pparams->pfunc_rx_complete;
   plist_new->pfunc_txrx_complete = pparams->pfunc_txrx_complete;
   plist_new->puser_data = pparams->puser_data;
   
   uint8_t res = JHAL_UART_INIT(plist_new->pinstance, pparams); 
   
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

uint8_t jhal_uart_deinit(void* pinstance)
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
   
  return JHAL_UART_DEINIT(pinstance);
}

uint8_t jhal_uart_transmit(void* pinstance, uint8_t* ptxdata, uint16_t size, uint32_t timeout)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !ptxdata || !size || !timeout) 
     return JHAL_RES_INVALID_PARAMS;
#endif
  return JHAL_UART_TRANSMIT(pinstance, ptxdata, size, timeout);
}

uint8_t jhal_uart_receive(void* pinstance, uint8_t* prxdata, uint16_t size, uint32_t timeout)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !prxdata || !size || !timeout) 
     return JHAL_RES_INVALID_PARAMS;
#endif  
  return JHAL_UART_RECEIVE(pinstance, prxdata, size, timeout);
}

uint8_t jhal_uart_transmitreceive(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, uint32_t timeout)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !ptxdata || !prxdata || !size || !timeout) 
     return JHAL_RES_INVALID_PARAMS;
#endif    
  return JHAL_UART_TRANSMITRECEIVE(pinstance, ptxdata, prxdata, size, timeout);
}

uint8_t jhal_uart_transmit_it(void* pinstance, uint8_t* ptxdata, uint16_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !ptxdata || !size) 
     return JHAL_RES_INVALID_PARAMS;
#endif    
  return JHAL_UART_TRANSMIT_IT(pinstance, ptxdata, size);
}

uint8_t jhal_uart_receive_it(void* pinstance, uint8_t* prxdata, uint16_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !prxdata || !size) 
     return JHAL_RES_INVALID_PARAMS;
#endif
  return JHAL_UART_RECEIVE_IT(pinstance, prxdata, size);
}

uint8_t jhal_uart_transmitreceive_it(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !ptxdata || !prxdata || !size) 
     return JHAL_RES_INVALID_PARAMS;
#endif
  return JHAL_UART_TRANSMITRECEIVE_IT(pinstance, ptxdata, prxdata, size);
}

uint8_t jhal_uart_transmit_dma(void* pinstance, uint8_t* ptxdata, uint16_t size, void* pinstance_dma)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !ptxdata || !size || !pinstance_dma) 
     return JHAL_RES_INVALID_PARAMS;
#endif  
  return JHAL_UART_TRANSMIT_DMA(pinstance, ptxdata, size, pinstance_dma);
}

uint8_t jhal_uart_receive_dma(void* pinstance, uint8_t* prxdata, uint16_t size, void* pinstance_dma)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !prxdata || !size || !pinstance_dma) 
     return JHAL_RES_INVALID_PARAMS;
#endif 
   return JHAL_UART_RECEIVE_DMA(pinstance, prxdata, size, pinstance_dma);
}

uint8_t jhal_uart_transmitreceive_dma(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, void* pinstance_dma)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
   if(!pinstance || !ptxdata || !prxdata || !size || !pinstance_dma) 
     return JHAL_RES_INVALID_PARAMS;
#endif  
  return JHAL_UART_TRANSMITRECEIVE_DMA(pinstance, ptxdata, prxdata, size, pinstance_dma);
}

void jhal_uart_tx_complete_callback(void* pinstance)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
  JHAL_ASSERT(pinstance);  
#endif
  instance_list* plist = plist_top;
  while(plist != NULL)
  {
    if(plist->pinstance == pinstance)
    {
      if(plist->pfunc_tx_complete)
        plist->pfunc_tx_complete(plist->puser_data);
      
      break;
    }
    plist = plist->pnext;
  }   
}

void jhal_uart_rx_complete_callback(void* pinstance, uint8_t* prxdata, uint16_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
  JHAL_ASSERT(pinstance && prxdata && size);  
#endif
  instance_list* plist = plist_top;
  while(plist != NULL)
  {
    if(plist->pinstance == pinstance)
    {
      if(plist->pfunc_rx_complete)
        plist->pfunc_rx_complete(plist->puser_data, prxdata, size);
      
      break;
    }
    plist = plist->pnext;
  }    
}

void jhal_uart_txrx_complete_callback(void* pinstance, uint8_t* prxdata, uint16_t size)
{
#if (JHAL_LEVEL_PROTECT > JHAL_LEVEL_PROTECT_LOW)
  JHAL_ASSERT(pinstance && prxdata && size);
#endif
  instance_list* plist = plist_top;
  while(plist != NULL)
  {
    if(plist->pinstance == pinstance)
    {
      if(plist->pfunc_txrx_complete)
        plist->pfunc_txrx_complete(plist->puser_data, prxdata, size);
      
      break;
    }
    plist = plist->pnext;
  }      
}