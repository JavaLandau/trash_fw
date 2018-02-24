/**
  \file    DeviceDS18B20.c 
  \brief   Исполняемый файл драйвера датчика температуры DS18B20
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "DeviceDS18B20.h"
#include "GPIOfun.h"
#include "TIMDelay.h"

/**
  \defgroup module_service_DS18B20 Служебные функции для работы с DS18B20
  \brief Модуль служебных функций, необходимых для работы с микросхемой датчика температуры DS18B20
@{
*/

#define COUNT_BITS_OF_BYTE              8               ///<Количество бит в байте

#define TIME_RESET_PULSE                460             ///<Время импульса Reset в микросекундах
#define TIME_PRESENCE_WAIT              15              ///<Время отклика от датчика в микросекундах

#define TIME_BETWEEN_SLOT               1               ///<Время между слотами в микросекундах

#define TIME_INIT_WRITE_SLOT            1               ///<Время инициализации слота записи в микросекундах
#define TIME_WRITE_0_SLOT               50              ///<Время записи "0" в микросекундах
#define TIME_WRITE_1_SLOT               35              ///<Время записи "1" в микросекундах

#define TIME_INIT_READ_SLOT             1               ///<Время инициализации слота чтения в микросекундах
#define TIME_READ_SLOT                  4               ///<Окно чтения
#define TIME_READ_SLOT_PART             35              ///<Время слота чтения

#define TIME_WAIT                       10              ///<Время ожидания

#define TIME_TEMP_CONV                  800000          ///<Время конвертации данных температуры

#define DS18B20_SCRATCHPAD_MEM_SIZE    9               ///<Размер памяти датчика температуры в байтах
#define DS18B20_COUNT_TRYING           10              ///<Количество попыток при запросе/отправке данных

static DeviceDS18B20 DS18B20Dev;                      ///<Объект драйвера датчика температуры
static uint8_t StatusReadConvertT = FALSE;              ///<Флаг состояния конвертации данных

/**Инициализация процесса обмена данными
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
static uint32_t prvDeviceDS18B20InitSequence(ExtCodeDeviceDS18B20* pExCode);

/**Чтение данных памяти датчика
  \param[out] pRecBytes считываемые данные
  \return Результат выполнения функции 
*/ 
static uint32_t prvDeviceDS18B20ReadScratchPad(uint8_t* pRecBytes);

/**Отправка данных в датчик 
  \param[in] pSendBytes отправляемые данные
  \param[in] Size размер данных
  \return Результат выполнения функции 
*/ 
static uint32_t prvDeviceDS18B20WriteBytes(uint8_t* pSendBytes, uint32_t Size);

/**Вычисление контрольной суммы CRC-8-Dallas/Maxim
  \param[in] Data отправляемые данные
  \param[in] Size размер данных
  \return Результат выполнения функции 
*/ 
static uint32_t prvDeviceDS18B20crc8(uint8_t* Data, uint32_t Size);

/*Вычисление контрольной суммы CRC-8-Dallas/Maxim*/
static uint32_t prvDeviceDS18B20crc8(uint8_t* Data, uint32_t Size)
{
  uint8_t crc = 0;
  for ( uint32_t i = 0; i < Size; ++i )
  {
      uint8_t inbyte = Data[i];
      for ( uint8_t j = 0; j < 8; ++j )
      {
          uint8_t mix = (crc ^ inbyte) & 0x01;
          crc >>= 1;
          if ( mix ) crc ^= 0x8C;
          inbyte >>= 1;
      }
  }
  return crc;
}

/*Инициализация процесса обмена данными*/
static uint32_t prvDeviceDS18B20InitSequence(ExtCodeDeviceDS18B20* pExCode)
{
  /*Проверка входных данных*/
  if(!pExCode)
    return FUNC_INVALID_PARAM;  
    
  *pExCode = DEVICE_DS18B20_NOT_CODE;  
  StatusReadConvertT = FALSE;
  
  /*Reset*/
  HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
  TIMDelay(TIME_RESET_PULSE);
  HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
  
  TIMDelay(TIME_WAIT);
  
  /*Время ожидания отклика от датчика*/
  uint8_t presenceFlag = FALSE;
   
  for(uint8_t i = 0; i < DS18B20_COUNT_TRYING; i++)
  {
    TIMDelay(TIME_PRESENCE_WAIT);
    
    if(HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum) == GPIO_PIN_RESET)
    {
      presenceFlag = TRUE; 
      break;
    }
  }
  
  if(!presenceFlag)
  {
    *pExCode = DEVICE_DS18B20_ERROR_NOT_RESP;
    return FUNC_ERROR;
  }
  
  /*Ожидание освобождения шины*/
  while(HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum) == GPIO_PIN_RESET); 
  
  return FUNC_OK;
}

/*Чтение данных памяти датчика*/
static uint32_t prvDeviceDS18B20ReadScratchPad(uint8_t* pRecBytes)
{
  /*Проверка входных данных*/
  if(!pRecBytes)
    return FUNC_INVALID_PARAM;     
  
  uint32_t Res;  
  uint8_t CurByte;
  
  /*Установка режима чтения*/
  uint8_t Cmd = SKIP_ROM;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);  
  
  Cmd = READ_SCRATCHPAD;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);
  
  /*Чтение данных из памяти*/
  if(Res == FUNC_OK)
  {
    for(uint8_t i = 0; i < DS18B20_SCRATCHPAD_MEM_SIZE; i++)
    {      
      CurByte = 0;
      uint8_t CurBit;
      for(uint8_t j = 0; j < COUNT_BITS_OF_BYTE; j++)
      {
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
        TIMDelay(TIME_INIT_READ_SLOT);
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
        TIMDelay(TIME_READ_SLOT);
        CurBit = HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum);
        
        TIMDelay(TIME_READ_SLOT_PART);
        
        if(CurBit == GPIO_PIN_SET)
          CurByte |= (1<<j);        
      }
      pRecBytes[i] = CurByte;
    }
  }
  
  return Res;
}

/*Отправка данных в датчик*/
static uint32_t prvDeviceDS18B20WriteBytes(uint8_t* pSendBytes, uint32_t Size)
{
  /*Проверка входных данных*/
  if(!pSendBytes)
    return FUNC_INVALID_PARAM;  
  
  StatusReadConvertT = FALSE;
  
  /*Запись данных*/
  for(uint32_t i = 0; i < Size; i++)  
  {
    uint8_t CurByte = pSendBytes[i];
    
    for(uint8_t j = 0; j < COUNT_BITS_OF_BYTE; j++)
    {
      HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
      if((CurByte >> j) & 0x1)
      {                        
        TIMDelay(TIME_INIT_WRITE_SLOT);        
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
        TIMDelay(TIME_WRITE_1_SLOT);        
      }
      else{        
        TIMDelay(TIME_WRITE_0_SLOT);
        HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
      }
      TIMDelay(TIME_BETWEEN_SLOT);
    }
  }
  
  return FUNC_OK;
}

/**
@}
  \defgroup module_DS18B20 Интерфейсные функции для работы с DS18B20
  \brief Модуль, предоставляющий пользователю необходимый функционал для работы с микросхемой датчика температуры DS18B20
@{
*/

/*Инициализация драйвера датчика температуры*/
uint32_t DeviceDS18B20Create(DeviceDS18B20Param* pDevParam, ExtCodeDeviceDS18B20* pExCode)
{  
  /*Проверка входных данных*/
  if(!pDevParam || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_DS18B20_NOT_CODE;
             
  /*Инициализация GPIO*/
  uint32_t Res;
  GPIO_InitTypeDef GPIO_InitStruct;
  DS18B20Dev.pGPIOTXPort = 0;
  
  Res = GPIOGetPort(pDevParam->GPIOTXPin,&DS18B20Dev.pGPIOTXPort,&DS18B20Dev.GPIOTXNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_DS18B20_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
    
  HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = DS18B20Dev.GPIOTXNum;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20Dev.pGPIOTXPort, &GPIO_InitStruct);    
  
  DS18B20Dev.pGPIORXPort = 0;
  Res = GPIOGetPort(pDevParam->GPIORXPin,&DS18B20Dev.pGPIORXPort,&DS18B20Dev.GPIORXNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_DS18B20_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
    
  GPIO_InitStruct.Pin = DS18B20Dev.GPIORXNum;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20Dev.pGPIORXPort, &GPIO_InitStruct);     
  
  StatusReadConvertT = FALSE;
  
  return FUNC_OK;  
}

/*Запрос состояния готовности выдачи данных о температуре*/
uint32_t DeviceDS18B20GetReadyTemperature(uint8_t* pTempReady)
{
  /*Проверка входных данных*/
  if(!pTempReady)
    return FUNC_INVALID_PARAM;  
     
  if(StatusReadConvertT)
  {
    HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_RESET);
    TIMDelay(TIME_INIT_READ_SLOT);
    HAL_GPIO_WritePin(DS18B20Dev.pGPIOTXPort, DS18B20Dev.GPIOTXNum, GPIO_PIN_SET);
    TIMDelay(TIME_READ_SLOT);
    *pTempReady = HAL_GPIO_ReadPin(DS18B20Dev.pGPIORXPort, DS18B20Dev.GPIORXNum);    
    TIMDelay(TIME_READ_SLOT_PART);     
  }else
    return FUNC_ERROR;  
  
  return FUNC_OK;
}

/*Запуск процесса конвертации данных о температуре*/
uint32_t DeviceDS18B20ConversionTemperature()
{    
  uint32_t Res;  
  uint8_t Cmd;
  uint8_t ReadyTemp;
  ExtCodeDeviceDS18B20 ExCode;   
  
  if(StatusReadConvertT && !DeviceDS18B20GetReadyTemperature(&ReadyTemp))
    return FUNC_ERROR;
      
  
  Res = prvDeviceDS18B20InitSequence(&ExCode);
  
  if(Res != FUNC_OK)
    return Res;

  Cmd = SKIP_ROM;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);
  
  Cmd = CONVERT_T;
  Res = prvDeviceDS18B20WriteBytes(&Cmd, 1);
  
  if(Res != FUNC_OK)
    return Res;

  StatusReadConvertT = TRUE;  
  return FUNC_OK;
}

/*Чтение температуры с датчика*/
uint32_t DeviceDS18B20GetTemperature(int8_t* pTemp, ExtCodeDeviceDS18B20* pExCode)
{
  /*Проверка входных данных*/
  if(!pExCode || !pTemp)
    return FUNC_INVALID_PARAM;  
    
  uint32_t Res;    
  uint8_t ArrayScratchPad[DS18B20_SCRATCHPAD_MEM_SIZE];
  
  *pExCode = DEVICE_DS18B20_NOT_CODE;    
  StatusReadConvertT = FALSE;
        
  Res = prvDeviceDS18B20InitSequence(pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  /*Чтение данных с памяти датчика*/  
  Res = prvDeviceDS18B20ReadScratchPad(ArrayScratchPad);
  
  if(Res != FUNC_OK)
    return Res;  
  
  uint32_t crc = prvDeviceDS18B20crc8(ArrayScratchPad, DS18B20_SCRATCHPAD_MEM_SIZE - 1);
  
  if(crc != ArrayScratchPad[DS18B20_SCRATCHPAD_MEM_SIZE - 1])
  {
    *pExCode = DEVICE_DS18B20_ERROR_CRC;
    return FUNC_ERROR;
  }
    
  uint16_t buffVar = (((int16_t)ArrayScratchPad[0])|(((int16_t)(ArrayScratchPad[1]))<<8))>>4;
  *pTemp = (int8_t)(buffVar & 0xFF);

  if(((ArrayScratchPad[0] >> 3)&0x1))
    (*pTemp)++;
      
  return FUNC_OK;
}

/**
@}
*/