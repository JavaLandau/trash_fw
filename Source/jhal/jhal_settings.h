#ifndef __JHAL_SETTINGS__
#define __JHAL_SETTINGS__

#ifdef __cplusplus
extern "C" {
#endif

#define USE_JHAL_ASSERT                 0  
#define USE_JHAL_OS                     0  
  
#define JHAL_LEVEL_PROTECT_LOW          0  
#define JHAL_LEVEL_PROTECT_MIDDLE       1
#define JHAL_LEVEL_PROTECT_HIGH         2
  
#define JHAL_LEVEL_PROTECT              JHAL_LEVEL_PROTECT_HIGH      
#define JHAL_SIZE_ADD_PARAMS            5
#define JHAL_SIZE_MEM                   128    
  
#ifdef __cplusplus
}
#endif

#endif