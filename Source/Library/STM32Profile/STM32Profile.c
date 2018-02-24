/**
  \file STM32Profile.c 
  \brief Исполняемый файл профилировщика STM32
  \author  Яковлев В. А.
  \version 1.0
  \date    25.01.2018 
*/

/**
  \defgroup module_STM32profile Профилировщик STM32
   
  \brief Модуль средств профилирования STM32. Это прототип 
         профилировщика для STM32, использующий по-умолчанию таймер TIM1

  \todo 
           - сделать аппаратно-независимым;
           - добавить возможность вносить калибровочное значение, 
             вычисленное по осциллографу;
           - добавить функции отсчета времени.
  @{
*/

#include "TypesDefine.h"
#include "stm32l4xx_hal.h"
#include "STM32Profile.h"

#define SCALE_NANO      1000000000.0    ///<Масштаб наносекунды
#define SCALE_MICRO     1000000.0       ///<Масштаб микросекунды
#define SCALE_MIL       1000.0          ///<Масштаб миллисекунды

#define SIZE_SELECTION  50              ///<Величина выборки

static STM32ProfileStruct ProfileObj;   ///<Объект профиля   

/** Запуск подсчета времени для калибровки задежрки, вносимой обработкой
    прерывания
*/
static void prvSTM32ProfileCalibrInterruptStart(void);

/*Инициализация профиля*/
uint32_t STM32ProfileInit(uint32_t FreqMCU, uint32_t Prescaler, uint32_t Period, STM32TimeScale TimeScale, STM32ProfileErrorCode* pErrorCode)
{
  /*Проверка входных параметров*/
  if(!pErrorCode)
    return FUNC_INVALID_PARAM;
  
  *pErrorCode = STM32_PROFILE_ERROR_TIM1;
  
  if(!Period || !FreqMCU)
    return FUNC_INVALID_PARAM;
  
  /*Заполнение структуры профиля*/  
  ProfileObj.TimerCycles = 0;
  ProfileObj.Period = Period;
  ProfileObj.TimerFreq = FreqMCU/(Prescaler + 1);
  
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  ProfileObj.htim.Instance = TIM1;
  ProfileObj.htim.Init.Prescaler = Prescaler;
  ProfileObj.htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  ProfileObj.htim.Init.Period = Period;
  ProfileObj.htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  ProfileObj.htim.Init.RepetitionCounter = 0;
  
  /*Инициализация таймера*/
  if (HAL_TIM_Base_Init(&ProfileObj.htim) != HAL_OK)
  {
    *pErrorCode = STM32_PROFILE_ERROR_TIM1;
    return FUNC_ERROR;
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&ProfileObj.htim, &sClockSourceConfig) != HAL_OK)
  {
    *pErrorCode = STM32_PROFILE_ERROR_TIM1;
    return FUNC_ERROR;
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&ProfileObj.htim, &sMasterConfig) != HAL_OK)
  {
    *pErrorCode = STM32_PROFILE_ERROR_TIM1;
    return FUNC_ERROR;
  }
  
  HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
     
  __HAL_TIM_CLEAR_IT(&ProfileObj.htim, TIM_IT_UPDATE);
  
  /*Калибровка профиля для учета всех задержек*/  
  ProfileObj.SelfDelay = 0;
  ProfileObj.SelfIntDelay = 0;
  ProfileObj.TimeScale = TIME_IN_CYCLES;
  STM32ProfileStart(); 
  STM32ProfileStop();
  
  uint32_t SumSelection = 0;  
  for(uint32_t i = 0; i < SIZE_SELECTION; i++)
  {
    STM32ProfileStart(); 
    SumSelection += STM32ProfileStop();
  }
  
  ProfileObj.SelfDelay = SumSelection/SIZE_SELECTION;
  
  SumSelection = 0;  
  for(uint32_t i = 0; i < SIZE_SELECTION; i++)
  {
    prvSTM32ProfileCalibrInterruptStart(); 
    SumSelection += (STM32ProfileStop() - ProfileObj.Period);
  }
  
  ProfileObj.SelfIntDelay = SumSelection/SIZE_SELECTION;
  ProfileObj.TimeScale = TimeScale;
  
  return FUNC_OK;
}

/*Запуск подсчета времени для калибровки задержки, вносимой обработкой
   прерывания*/
static void prvSTM32ProfileCalibrInterruptStart(void)
{ 
  ProfileObj.TimerCycles = 0;
  __HAL_TIM_SET_COUNTER(&ProfileObj.htim, 0xFFFE);  
  HAL_TIM_Base_Start_IT(&ProfileObj.htim);  
}

/*Запуск подсчета времени*/
void STM32ProfileStart(void)
{
  ProfileObj.TimerCycles = 0;
  __HAL_TIM_SET_COUNTER(&ProfileObj.htim, 0);  
  HAL_TIM_Base_Start_IT(&ProfileObj.htim);  
}

/*Завершение процесса подсчета времени*/
uint32_t STM32ProfileStop(void)
{ 
  uint32_t CurCount;  
  CurCount = __HAL_TIM_GET_COUNTER(&ProfileObj.htim);
  
  HAL_TIM_Base_Stop_IT(&ProfileObj.htim);
  if(CurCount >= ProfileObj.TimerCycles)
    ProfileObj.TimerCycles = CurCount;
  else
    ProfileObj.TimerCycles += CurCount;
  
  if(ProfileObj.TimerCycles < ProfileObj.SelfDelay)
    ProfileObj.TimerCycles = 0;
  else
    ProfileObj.TimerCycles -= ProfileObj.SelfDelay;
  
  switch(ProfileObj.TimeScale)
  {
    case TIME_IN_CYCLES:
      return ProfileObj.TimerCycles;
    case TIME_IN_NSEC:
      return (uint32_t)(((float)ProfileObj.TimerCycles/(float)ProfileObj.TimerFreq)*SCALE_NANO);      
    case TIME_IN_UCSEC: 
      return (uint32_t)(((float)ProfileObj.TimerCycles/(float)ProfileObj.TimerFreq)*SCALE_MICRO);
    case TIME_IN_MILSEC:
      return (uint32_t)(((float)ProfileObj.TimerCycles/(float)ProfileObj.TimerFreq)*SCALE_MIL);
    default:
      return ProfileObj.TimerCycles/ProfileObj.TimerFreq;     
  }
}

/**Обработчик прерываний от таймера TIM1*/
void TIM1_UP_TIM16_IRQHandler(void)
{
  __HAL_TIM_CLEAR_IT(&ProfileObj.htim, TIM_IT_UPDATE);
  ProfileObj.TimerCycles+=(ProfileObj.Period - ProfileObj.SelfIntDelay);
}
/**  
  @}
*/