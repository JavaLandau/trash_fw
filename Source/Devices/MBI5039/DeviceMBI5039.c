/**
  \file    DeviceMBI5039.c 
  \brief   Исполняемый файл драйвера светодиодов MBI5039
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/
#include <stdlib.h>
#include "DeviceMBI5039.h"
#include "GPIOfun.h"

/**
  \defgroup module_MBI5039 Интерфейсные функции для работы с MBI5039
  \brief Модуль, предоставляющий пользователю необходимый функционал для работы с драйвером светодиодов MBI5039
@{
*/

///Время ожидания окончания приёма/передачи данных по SPI
#define OS_TIME_WAIT_SPI        100

///Длина импульса сигнала NSS для защелкивания данных
#define NSS_PULSE_WIDTH         5

///Формирование импульса сигнала NSS для защелкивания данных
#define NSS_PULSE() for(uint32_t SPIIncrement = 0; SPIIncrement < NSS_PULSE_WIDTH; SPIIncrement++) asm("NOP");


/*Инициализация драйвера светодиодов*/ 
uint32_t DeviceMBI5039Create(DeviceMBI5039Param* pDevParam, DeviceMBI5039* pDev, ExtCodeDeviceMBI5039* pExCode)
{  
  /*Проверка входных данных*/
  if(!pDevParam || !pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_MBI5039_NOT_CODE;
      
  if(!pDevParam->pInstanceSPIDrv)
    return FUNC_INVALID_PARAM;
  
  if(!pDevParam->devNum)
    return FUNC_INVALID_PARAM;    
  
  /*Инициализация SPI*/
  pDev->SPIDrv.State = HAL_SPI_STATE_RESET;
  pDev->SPIDrv.Instance = pDevParam->pInstanceSPIDrv;
  pDev->SPIDrv.Init.Mode = SPI_MODE_MASTER;
  pDev->SPIDrv.Init.Direction = SPI_DIRECTION_2LINES;
  pDev->SPIDrv.Init.DataSize = SPI_DATASIZE_16BIT;
  pDev->SPIDrv.Init.CLKPolarity = SPI_POLARITY_LOW;
  pDev->SPIDrv.Init.CLKPhase = SPI_PHASE_1EDGE;
  pDev->SPIDrv.Init.NSS = SPI_NSS_HARD_OUTPUT;
  pDev->SPIDrv.Init.BaudRatePrescaler = pDevParam->BaudRateSPIPrescaler;
  pDev->SPIDrv.Init.FirstBit = SPI_FIRSTBIT_MSB;
  pDev->SPIDrv.Init.TIMode = SPI_TIMODE_DISABLE;
  pDev->SPIDrv.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  pDev->SPIDrv.Init.CRCPolynomial = 7;
  pDev->SPIDrv.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  pDev->SPIDrv.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&((pDev)->SPIDrv)) != HAL_OK)
  {
    *pExCode = DEVICE_MBI5039_ERROR_INIT_SPI;
    return FUNC_ERROR;
  }
   
  /*Инициализация NSS*/
  uint32_t Res;
  GPIO_InitTypeDef GPIO_InitStruct;
  pDev->pGPIONSSPort = 0;
  
  Res = GPIOGetPort(pDevParam->GPIONSSPin,&pDev->pGPIONSSPort,&pDev->GPIONSSNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_MBI5039_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
    
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = pDev->GPIONSSNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(pDev->pGPIONSSPort, &GPIO_InitStruct);    
  
  pDev->devNum = pDevParam->devNum;
  
  return FUNC_OK;  
}

/*Установка выходов драйвера*/
uint32_t DeviceMBI5039Set(DeviceMBI5039* pDev, uint16_t* pOutput, ExtCodeDeviceMBI5039* pExCode)
{
  /*Проверка входных данных*/
  if(!pDev || !pExCode || !pOutput)
    return FUNC_INVALID_PARAM;  
    
  *pExCode = DEVICE_MBI5039_NOT_CODE;

  HAL_StatusTypeDef HALRes;  
  
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_RESET);  
  HALRes = HAL_SPI_Transmit(&pDev->SPIDrv, (uint8_t*)pOutput, pDev->devNum*sizeof(uint8_t), OS_TIME_WAIT_SPI); 
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_SET); 
  
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_RESET);  
  NSS_PULSE();
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_SET);  
  
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_MBI5039_ERROR_WRITE_SPI;
    return FUNC_ERROR;
  }
  
  return FUNC_OK;
}

/*Сброс выходов драйвера*/
uint32_t DeviceMBI5039Reset(DeviceMBI5039* pDev,  ExtCodeDeviceMBI5039* pExCode)
{
  /*Проверка входных данных*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
    
  *pExCode = DEVICE_MBI5039_NOT_CODE;
  uint16_t* WriteData = (uint16_t*)malloc(pDev->devNum*sizeof(uint16_t));
  
  if(!WriteData)
    return FUNC_HEAP_MEM_ERROR;
  
  /*Сброс выходов драйвера*/
  HAL_StatusTypeDef HALRes;  
  
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_RESET);    
  HALRes = HAL_SPI_Transmit(&pDev->SPIDrv, (uint8_t*)&WriteData, pDev->devNum, OS_TIME_WAIT_SPI); 
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_SET);  
  
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_RESET);  
  NSS_PULSE();
  HAL_GPIO_WritePin(pDev->pGPIONSSPort, pDev->GPIONSSNum, GPIO_PIN_SET);  
  
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_MBI5039_ERROR_WRITE_SPI;
    free(WriteData);
    return FUNC_ERROR;
  }
  
  free(WriteData);
  return FUNC_OK;
}

/**
@}
*/