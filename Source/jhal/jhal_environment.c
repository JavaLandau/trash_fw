#include <stdio.h>
#include "jhal_environment.h"

#define STATIC_MEM_SERVICE_BYTES        5

static uint32_t watermark = 0;
static uint8_t Mem[JHAL_SIZE_MEM] = {0};

#pragma pack(push, 1)
struct _instance_list{
  uint8_t                             invalid;
  struct _instance_list*              pnext;
  void*                               pinstance;  
};
#pragma pack(pop)

typedef struct _instance_list instance_list;

jhal_driver_instance* jhal_driver_malloc(uint32_t size_instance, uint32_t size_driver)
{
    void* alloced_instance = jhal_malloc(sizeof(jhal_driver_instance) + size_instance + size_driver);
    if(alloced_instance != NULL)
    {
        uint32_t address = JHAL_OFFSET(alloced_instance, jhal_driver_instance, pinstance);
        *((void**)address) = (void*)((uint32_t)alloced_instance + sizeof(alloced_instance));
        
        address = JHAL_OFFSET(alloced_instance, jhal_driver_instance, pfuncs_callbacks);
        *((void**)address) = (void*)((uint32_t)alloced_instance + sizeof(alloced_instance) + size_driver);
        
        address = JHAL_OFFSET(alloced_instance, jhal_driver_instance, instance_signature);
        *((uint32_t*)address) = INSTANCE_SIGNATURE;
    }

    return (jhal_driver_instance*)alloced_instance;
}

void jhal_driver_free(void* pinstance)
{
    jhal_free(pinstance);
    JHAL_GET_SIGNATURE(pinstance) = 0;
}

void* jhal_malloc(uint32_t size)
{   
   instance_list* plist = NULL;
   if(!watermark)
   {
     if(watermark + size + STATIC_MEM_SERVICE_BYTES  > JHAL_SIZE_MEM)
       return NULL;
     
     plist = (instance_list*)Mem;
     plist->pinstance = (void*)&Mem[watermark];
     watermark += size + STATIC_MEM_SERVICE_BYTES;
     plist->pnext = NULL;     
   } else 
   {
     plist = (instance_list*)Mem;
     while(1)
     {
       while(!plist->invalid && plist->pnext != NULL)
         plist = plist->pnext;
       
       if(!plist->invalid)
       {
         if(watermark + size + STATIC_MEM_SERVICE_BYTES  > JHAL_SIZE_MEM)
           return NULL;
                
         plist->pnext = (instance_list*)&Mem[watermark];
         plist = plist->pnext;
         plist->pinstance = (void*)&Mem[watermark];
         watermark += size + STATIC_MEM_SERVICE_BYTES;
         plist->pnext = NULL;
       }else
       {
         if(plist->pnext == NULL)
         {
           uint32_t prev_size = (uint32_t)&Mem[watermark] - (uint32_t)plist + 1;
           if((size > prev_size) && (size - prev_size + watermark  > JHAL_SIZE_MEM))
             return NULL;

           watermark += size - prev_size;
         }else
         {
           if(plist - plist->pnext + 1 >= size)
             break;
           else
             plist = plist->pnext;
         }
       }
     }
   }
      
   plist->invalid = 0;  
   return plist;
}

void jhal_free(void* pinstance)
{
  instance_list* plist = (instance_list*)Mem;
  while(plist != NULL)
  {
    if(plist == pinstance)
    {
      plist->invalid = 1;
      break;
    }
    plist = plist->pnext;
  }
}