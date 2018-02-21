/**
  \file    stm8s_clk.h 
  \brief   Заголовочный файл драйвера системы тактирования ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_CLK__
#define __STM8S_CLK__

#include "TypesDefine.h"

/**Назначение HSE источником тактирования ядра 
  \return Результат выполнения функции
*/
uint32_t stm8s_clk_SetHSEMasterClock(void);

#endif