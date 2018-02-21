/**
  \file    stm8s_flash.h 
  \brief   Заголовочный файл драйвера внутренней флеш-памяти STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_FLASH__
#define __STM8S_FLASH__

#include "TypesDefine.h"

/**Разрешение доступа к option bytes для флеш-памяти
  \return Результат выполнения функции
*/
uint32_t stm8s_flash_OptionBytesEnable(void);

/**Запрет доступа к option bytes для флеш-памяти
  \return Результат выполнения функции
*/
uint32_t stm8s_flash_OptionBytesDisable(void);

#endif
