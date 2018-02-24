/**
  \file    main.c 
  \brief   Исполняемый файл программы управления светодиодным градусником
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "main.h"
#include "stm32f0xx_hal.h"
#include "DeviceDS18B20.h"
#include "AlcoholColumn.h"
#include "TIMDelay.h"
#include "TypesDefine.h"

///Величина задержки при периодическом опросе
#define SYS_DELAY       400000

/**Конфигурация системы тактирования*/
void SystemClock_Config(void);

/**Программа управления светодиодным градусником
  \return Результат выполнения функции 
*/ 
int main(void)
{
  uint32_t Res = FUNC_OK;

  HAL_Init();
  SystemClock_Config();
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_TIM1_CLK_ENABLE();  
  
  /*Инициализация таймера задержки*/
  Res = InitTIM(48);
  
  if(Res != FUNC_OK)
    return 0;
  
  /*Инициализация модуля управления светодиодным спиртовым столбом*/
  Res = AlcoholColumnCreate();  
  
  if(Res == FUNC_OK)
  {
    /*Инициализация драйвера датчика температуры*/
    DeviceDS18B20Param DevParam;
    ExtCodeDeviceDS18B20 DS18B20ExCode;
    
    DevParam.GPIOTXPin = "PA10";
    DevParam.GPIORXPin = "PA9";
    
    Res = DeviceDS18B20Create(&DevParam, &DS18B20ExCode);  
    
    if(Res == FUNC_OK)
    {      
      int8_t Temp, OverflowTemp;
      uint8_t ReadyTemp;
      int8_t Blink = -1;
      
      Res = DeviceDS18B20ConversionTemperature();
      
      if(Res != FUNC_OK)
        return 0;
      
      /*Периодический опрос датчика*/
      while (1)
      {      
        DeviceDS18B20GetReadyTemperature(&ReadyTemp);
        
        if(ReadyTemp)
        {
          Res = DeviceDS18B20GetTemperature(&Temp, &DS18B20ExCode);
          
          if(Res == FUNC_OK)
            AlcoholColumnSet(Temp);          
          
          /*Мигание светодиодного столба в случае выхода за температурный диапазон*/
          if(Temp > TEMPERATURE_MAX || Temp < TEMPERATURE_MIN)
          {
            Blink = 0;
            if(Temp > TEMPERATURE_MAX)
              OverflowTemp = TEMPERATURE_MAX;
            else
              OverflowTemp = TEMPERATURE_MIN;
          }else
            Blink = -1;
          
          DeviceDS18B20ConversionTemperature();          
        }

        if(Blink != -1)
        {
          Blink = 1 - Blink;
          
          if(Blink)
            AlcoholColumnSet(OverflowTemp);
          else
            AlcoholColumnReset();                     
        }
        
        TIMDelay(SYS_DELAY);
      }
    }
  }
}

/*Конфигурация системы тактирования*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    return;

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    return;

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    return;
 
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  
  HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1, 0);
}