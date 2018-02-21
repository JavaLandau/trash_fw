/**
  \file    DeviceEMS22A.c 
  \brief   Исполняемый файл драйвера абсолютного энкодера EMS22A
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "DeviceEMS22A.h"

/**
  \defgroup module_service_EMS22A Служебные функции для работы с EMS22A
  \brief Модуль служебных функций, необходимых для работы с абсолютным энкодером EMS22A
@{
*/

///Время ожидания окончания приёма/передачи данных по SPI
#define EMS22A_TIME_WAIT_SPI            1000                    

///Максимально возможный возращаемый энкодером код угла
#define EMS22A_FULL_RANGE_CODE          0x400                   

///Количество градусов в полном круге
#define EMS22A_FULL_ROTATE_DEG          360

#define GET_BIT(VALUE,NUM)              (((VALUE)>>(NUM))&0x1)  ///<Значение заданного бита
#define ANGULAR_BITS(VALUE)             (((VALUE)>>0x6)&0x3FF)  ///<Значение угла
#define STATUS_BITS(VALUE)              (((VALUE)>>0x1)&0x1F)   ///<Значение бита статусу
#define PARITY_BIT(VALUE)               ((VALUE)&0x1)           ///<Значение бита контрольной суммы

/**Проверка контрольной суммы данных с энкодера
  \param[in] Value данные с датчика
  \return Результат сравнения контрольных сумм (1 - совпадают, 0 - не совпадают)
*/ 
static uint8_t prvDeviceEMS22AEventParity(uint16_t Value)
{
  uint8_t ParityValue = 0;
  for(uint8_t i = 0; i < 16; i++)
    ParityValue+=GET_BIT(Value,i);
  
  return !(ParityValue % 2);
}

/**Проверка связи с энкодером
  \param[in] pDev указатель на структуру драйвера
  \return Результат выполнения функции
*/ 
static uint32_t prvDeviceEMS22ACheck(DeviceEMS22A* pDev)
{
  /*Проверка входных данных*/
  if(!pDev)
    return FUNC_INVALID_PARAM;  
  
  HAL_StatusTypeDef HALRes;
  uint16_t ReceiveHalfWord = 0xFFFF;
    
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Receive(&pDev->SPIDrv, (uint8_t*)&ReceiveHalfWord, 1, EMS22A_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  
  if(HALRes != HAL_OK)    
    return FUNC_ERROR;

  if(!prvDeviceEMS22AEventParity(ReceiveHalfWord))
    return FUNC_ERROR;    
  
  return FUNC_OK;
}

/**
@}
  \defgroup module_EMS22A Интерфейсные функции для работы с EMS22A
  \brief Модуль, предоставляющий пользователю необходимый функционал для работы с абсолютным энкодером EMS22A
@{
*/

/*Инициализация драйвера энкодера*/
uint32_t DeviceEMS22ACreate(DeviceEMS22AParam* pDevParam, DeviceEMS22A* pDev, ExtCodeDeviceEMS22A* pExCode)
{
  /*Проверка входных данных*/
  if(!pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_EMS22A_NOT_CODE;
  
  if(!pDevParam || !pDev)
    return FUNC_INVALID_PARAM;
    
  /*Инициализация SPI*/
  pDev->SPIDrv.State = HAL_SPI_STATE_RESET;
  pDev->SPIDrv.Instance = pDevParam->pInstanceSPIDrv;
  pDev->SPIDrv.Init.Mode = SPI_MODE_MASTER;
  pDev->SPIDrv.Init.Direction = SPI_DIRECTION_2LINES;
  pDev->SPIDrv.Init.DataSize = SPI_DATASIZE_16BIT;
  pDev->SPIDrv.Init.CLKPolarity = SPI_POLARITY_LOW;
  pDev->SPIDrv.Init.CLKPhase = SPI_PHASE_2EDGE;
  pDev->SPIDrv.Init.NSS = SPI_NSS_SOFT;
  pDev->SPIDrv.Init.BaudRatePrescaler = pDevParam->BaudRateSPIPrescaler;
  pDev->SPIDrv.Init.FirstBit = SPI_FIRSTBIT_MSB;
  pDev->SPIDrv.Init.TIMode = SPI_TIMODE_DISABLE;
  pDev->SPIDrv.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  pDev->SPIDrv.Init.CRCPolynomial = 10;
  pDev->SPIDrv.pUserData = (void*)pDev;
  if (HAL_SPI_Init(&pDev->SPIDrv) != HAL_OK)
  {
    *pExCode = DEVICE_EMS22A_ERROR_INIT_SPI;
    return FUNC_ERROR;
  }

  /*Проверка связи с энкодером*/
  if(prvDeviceEMS22ACheck(pDev) != FUNC_OK)
    return FUNC_ERROR;
  
  return FUNC_OK;  
}

/*Чтение данных с энкодера*/
uint32_t DeviceEMS22AGetCode(DeviceEMS22A* pDev, uint16_t* pCode, ExtCodeDeviceEMS22A* pExCode)
{
  /*Проверка входных данных*/
  if(!pDev || !pCode || !pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_EMS22A_NOT_CODE;  
  
  HAL_StatusTypeDef HALRes;
  uint16_t ReceiveHalfWord = 0xFFFF;
    
  /*Чтение данных*/
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
  HALRes = HAL_SPI_Receive(&pDev->SPIDrv, (uint8_t*)&ReceiveHalfWord, 1, EMS22A_TIME_WAIT_SPI);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  
  if(HALRes != HAL_OK)
  {
    *pExCode = DEVICE_EMS22A_ERROR_PROCESS_SPI;
    return FUNC_ERROR;
  }

  /*Проверка CRC*/
  if(!prvDeviceEMS22AEventParity(ReceiveHalfWord))
  {
    *pExCode = DEVICE_EMS22A_ERROR_CRC;
    return FUNC_ERROR;    
  }
  
  *pCode = ANGULAR_BITS(ReceiveHalfWord);
      
  return FUNC_OK;  
}

/*Чтение значения угла поворота*/
uint32_t DeviceEMS22AGetAngular(DeviceEMS22A* pDev, float* pAng, ExtCodeDeviceEMS22A* pExCode)
{
  /*Проверка входных данных*/
  if(!pDev || !pAng || !pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_EMS22A_NOT_CODE;  
  
  uint32_t Res;
  uint16_t EncoderCode;
  
  /*Чтение данных с энкодера*/
  Res = DeviceEMS22AGetCode(pDev, &EncoderCode, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  /*Вычисление угла поворота*/
  *pAng = EMS22A_FULL_ROTATE_DEG*(float)EncoderCode /(float)EMS22A_FULL_RANGE_CODE;
      
  return FUNC_OK;
}

/**
@}
*/