/**
  \file    stm8s_optionbytes.h
  \brief   Заголовочный файл драйвера управления Option Bytes ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_OPTIONBYTES__
#define __STM8S_OPTIONBYTES__

#include "TypesDefine.h"

/** @name Варианты настроек доступа к флеш-памяти
*/                        
///@{
#define OPT7_WAIT_0			0
#define OPT7_WAIT_1			1
///@}

/**Настройка доступа к флеш-памяти
  \param[in] WaitState вариант доступа к флеш-памяти
  \return Результат выполнения функции
*/
uint32_t stm8s_optionbytes_SetFlashWait(char WaitState);

#endif