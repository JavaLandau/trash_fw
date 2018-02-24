/**
  \file    TIMDelay.c 
  \brief   Исполняемый файл таймера задержки
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "stm32f0xx_hal.h"
#include "TypesDefine.h"

///Объект таймера задержки
static TIM_HandleTypeDef TIMDriver;

///Счетчик для подсчета времени задержки
static int32_t TIMCount = 0;

/**Обработчик прерываний от таймер TIM1*/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
  __HAL_TIM_CLEAR_IT(&TIMDriver, TIM_IT_UPDATE);
  TIMCount--;
}

/*Задержка на заданное время*/
void TIMDelay(uint32_t microsec)
{   
  TIMCount = microsec;  
  HAL_TIM_Base_Start_IT(&TIMDriver);  
  while(TIMCount > 0);  
  HAL_TIM_Base_Stop_IT(&TIMDriver);
}

/*Инициализация таймера задержки*/
uint32_t InitTIM(uint32_t period)
{    
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
   
  TIMDriver.State = HAL_TIM_STATE_RESET;
  TIMDriver.Instance = TIM1;
  TIMDriver.Init.Prescaler = 0;
  TIMDriver.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIMDriver.Init.Period = period;
  TIMDriver.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TIMDriver.Init.RepetitionCounter = 0;
  TIMDriver.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&TIMDriver) != HAL_OK)
     return FUNC_ERROR;

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&TIMDriver, &sClockSourceConfig) != HAL_OK)
     return FUNC_ERROR;

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&TIMDriver, &sMasterConfig) != HAL_OK)
     return FUNC_ERROR;
      
  return FUNC_OK;
}