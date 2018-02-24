/**
  \file    stm8s_tim.h
  \brief   Заголовочный файл драйвера таймера ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_TIM__
#define __STM8S_TIM__

#include "TypesDefine.h"

///Номер таймера
typedef enum _TIMTypeObj {
  TIM1 = 0x005250,                      ///<TIM1
  TIM2 = 0x005300,                      ///<TIM2
  TIM3 = 0x005320,                      ///<TIM3
  TIM4 = 0x005340                       ///<TIM4
} TIMTypeObj;

///Структура таймера
typedef struct _TIMType  {
  TIMTypeObj        Instance;           ///<Номер таймера
  HwrIntFunc        FuncCallback;       ///<Указатель на обработчик прерываний от таймера
  void*             UserData;           ///<Указатель на пользовательские данные
  unsigned int      Prescaler;          ///<Значение предделителя
  unsigned int      ARR;                ///<Значение периода счетчика
} TIMType;

/**Инициализация таймера
  \param[in] TIMDef структура таймера
  \return Результат выполнения функции
*/
uint32_t stm8s_tim_Init(TIMType* TIMDef);

/**Запуск счетчика таймера
  \param[in] TIMDef структура таймера
  \return Результат выполнения функции
*/
uint32_t stm8s_tim_Start(TIMType* TIMDef);

/**Остановка счетчика таймера
  \param[in] TIMDef структура таймера
  \return Результат выполнения функции
*/
uint32_t stm8s_tim_Stop(TIMType* TIMDef);

/**Обработчик прерываний по событию UPDATE от таймера TIM1*/
void stm8s_TIM1_UpdateInterrupt(void);

/**Обработчик прерываний по событию UPDATE от таймера TIM2*/
void stm8s_TIM2_UpdateInterrupt(void);

/**Обработчик прерываний по событию UPDATE от таймера TIM3*/
void stm8s_TIM3_UpdateInterrupt(void);

#endif