#ifndef __DEVICE_LCD1602_GPIO__
#define __DEVICE_LCD1602_GPIO__

#define LCD1602_LEN_DATA        8

typedef struct {
  uint64_t      pinsData[LCD1602_LEN_DATA];
  uint64_t      pinRS;
  uint64_t      pinRW;  
  uint64_t      pinE;  
} DeviceLCD1602_GPIOParams;

uint8_t LCD1602Init_GPIO(void** ppInstance, void* pParams);
uint8_t LCD1602ReadWrite_GPIO(void* pInstance, void* pParams, uint8_t* pData, uint8_t valueRS, 
                              uint8_t valueRW);
uint8_t LCD1602EnableDisable_GPIO(void* pInstance, void* pParams, uint8_t valueE);
uint8_t LCD1602Deinit_GPIO(void* pInstance);

#endif