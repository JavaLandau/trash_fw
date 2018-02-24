/**
  \file    DevicePCA9554.c 
  \brief   Исполняемый файл расширителя портов GPIO PCA9554
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include <stdlib.h>
#include <stdio.h>

#include "DevicePCA9554.h"
#include "TypesDefine.h"
#include "GPIOfun.h"

/**
  \defgroup module_service_PCA9554 Служебные функции для работы с PCA9554
  \brief Модуль служебных функций, необходимых для работы с расширителем портов GPIO PCA9554
@{
*/

///Тип микросхемы
#if !defined(PCA9554) && !defined(PCA9554A)
#define PCA9554
#endif

///Старший адрес \f$I^2C\f$  микросхемы
#ifdef PCA9554
#define HIGH_ADDRESS                            (0x20 << 0x1)
#else
#define HIGH_ADDRESS                            (0x38 << 0x1)
#endif

///Номер порта GPIO
#define PORT_PIN(NUM)                           (1<<(NUM))	                      

///Изменение отдельного бита в байте
#define SET_BIT_OF_BYTE(BYTE, BIT, NUM)         if(BIT) BYTE|=(uint8_t)(1<<(NUM)); else BYTE&=(uint8_t)(~(1<<(NUM))); 

///Время ожидания ответа от устройства \f$I^2C\f$	
#define OS_TIME_WAIT_I2C                		100	

///Длина пакета \f$I^2C\f$ в байтах
#define LENGTH_PACKET                   		2	

///Тип команды
typedef enum _DevicePCA9554TypeCmd {
  CMD_INPUT_PORT_REG = 0x0,                             ///<Запросить состояние входа
  CMD_OUTPUT_PORT_REG,                                  ///<Задать состояние выхода  
  CMD_POLARITY_INVERSION_PORT_REG,                      ///<Инвертировать состояние входа/выхода
  CMD_CONF_REG                                          ///<Задать конфигурацию портов 
} DevicePCA9554TypeCmd;


/**Запись в регистр PCA9554
  \param[in] pDev указатель на структуру драйвера
  \param[in] TypeCmd тип команды
  \param[in] ConfByte значение регистра
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDevicePCA9554WriteRegister(DevicePCA9554* pDev, DevicePCA9554TypeCmd TypeCmd, uint8_t ConfByte, ExtCodeDevicePCA9554* pExCode);

/**Чтение регистра портов GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[out] pInputPortReg значение регистра портов GPIO
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDevicePCA9554ReadRegister(DevicePCA9554* pDev, uint8_t* pInputPortReg, ExtCodeDevicePCA9554* pExCode);

/**
@}
  \defgroup module_PCA9554 Интерфейсные функции для работы с PCA9554
  \brief Модуль, предоставляющий пользователю необходимый функционал для работы с расширителем портов GPIO PCA9554
@{
*/

/* Инициализация драйвера PCA9554*/
uint32_t DevicePCA9554Create(DevicePCA9554Param* pDevParam, DevicePCA9554* pDev, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDevParam || !pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;
    
  if(!pDevParam->pInstanceI2CDrv)
    return FUNC_INVALID_PARAM;
  
  uint32_t Res;  
    
  pDev->DevAddress = pDevParam->AddrDevice | HIGH_ADDRESS; 
  
  /*Инициализация I2C*/
  pDev->I2CDrv.State = HAL_I2C_STATE_RESET;
  pDev->I2CDrv.Instance = pDevParam->pInstanceI2CDrv;
  pDev->I2CDrv.Init.Timing = 0x2010091A;
  pDev->I2CDrv.Init.OwnAddress1 = 0;
  pDev->I2CDrv.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  pDev->I2CDrv.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  pDev->I2CDrv.Init.OwnAddress2 = 0;
  pDev->I2CDrv.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  pDev->I2CDrv.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  pDev->I2CDrv.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&pDev->I2CDrv) != HAL_OK)
  {
    *pExCode = DEVICE_PCA9554_ERROR_INIT_I2C;    
    return FUNC_ERROR;
  }

  /*Конфигурирование аналогового фильтра*/
  if (HAL_I2CEx_ConfigAnalogFilter(&pDev->I2CDrv, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    *pExCode = DEVICE_PCA9554_ERROR_INIT_I2C;
    return FUNC_ERROR;
  }

  /*Конфигурирование цифрового фильтра*/  
  if (HAL_I2CEx_ConfigDigitalFilter(&pDev->I2CDrv, 0) != HAL_OK)
  {
    *pExCode = DEVICE_PCA9554_ERROR_INIT_I2C;
    return FUNC_ERROR;
  }
  
  /*Инициализация GPIO*/
  GPIO_InitTypeDef GPIO_InitStruct;
  pDev->pGPIOINTPort = NULL;
  
  Res = GPIOGetPort(pDevParam->GPIOINTPin,&pDev->pGPIOINTPort,&pDev->GPIOINTNum);
  
  if(Res != FUNC_OK)
  {
    *pExCode = DEVICE_PCA9554_ERROR_INIT_GPIO;
    return FUNC_ERROR;    
  }
  
  GPIO_InitStruct.Pin = pDev->GPIOINTNum;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(pDev->pGPIOINTPort, &GPIO_InitStruct);   
  
  return FUNC_OK;
}

/*Конфигурирование отдельных портов GPIO*/
uint32_t DevicePCA9554SetConfPort(DevicePCA9554* pDev, uint8_t NumPort, uint8_t PortDirection, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;  
  
  if(NumPort > PCA9554_NUM_GPIO - 1)
    return FUNC_INVALID_PARAM;  
  
  if(PortDirection != PORT_PIN_INPUT && PortDirection != PORT_PIN_OUTPUT)
    return FUNC_INVALID_PARAM;  
  
  /*Конфигурирование порта GPIO*/
  uint32_t Res;
  uint8_t ConfByte = pDev->ConfRegister;
  SET_BIT_OF_BYTE(ConfByte, PortDirection, NumPort);
  
  Res = prvDevicePCA9554WriteRegister(pDev, CMD_CONF_REG, ConfByte, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  pDev->ConfRegister = ConfByte;
    
  return FUNC_OK;  
}

/*Конфигурирование всех портов GPIO*/
uint32_t DevicePCA9554SetConfRegister(DevicePCA9554* pDev, uint8_t* pPortDirection, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pPortDirection || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;    
  
  /*Конфигурирование портов GPIO*/
  uint8_t ConfByte = 0x0;
  uint32_t Res;
  
  for(uint8_t i = 0; i < PCA9554_NUM_GPIO; i++)    
      ConfByte |= (pPortDirection[i] << i);
  
  Res = prvDevicePCA9554WriteRegister(pDev, CMD_CONF_REG, ConfByte, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  pDev->ConfRegister = ConfByte;
    
  return FUNC_OK;
}

/*Считывание значений всех входов GPIO*/
uint32_t DevicePCA9554GetAllInputPorts(DevicePCA9554* pDev, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode || !pPortState)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;  
  
  /*Считывание значений всех входов GPIO*/
  uint32_t Res;    
  uint8_t InputByte;
    
  Res = prvDevicePCA9554ReadRegister(pDev, &InputByte, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
    
  for(uint8_t i = 0; i < PCA9554_NUM_GPIO; i++)
  {
    if(InputByte & PORT_PIN(i))
      pPortState[i] = GPIO_PIN_SET;
    else
      pPortState[i] = GPIO_PIN_RESET;    
  }
  
  return FUNC_OK;      
}

/*Считывание значения отдельного входа GPIO*/
uint32_t DevicePCA9554GetInputPort(DevicePCA9554* pDev, uint8_t NumPort, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode || !pPortState)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;  
  
  if(NumPort > PCA9554_NUM_GPIO - 1)
    return FUNC_INVALID_PARAM;
  
  if(((PORT_PIN(NumPort) & pDev->ConfRegister)>>NumPort) == PORT_PIN_OUTPUT)
    return FUNC_INVALID_PARAM;
  
  /*Считывание значения входа GPIO*/
  uint32_t Res;
  uint8_t InputByte;
    
  Res = prvDevicePCA9554ReadRegister(pDev, &InputByte, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
    
  if(InputByte & PORT_PIN(NumPort))
    *pPortState = GPIO_PIN_SET;
  else
    *pPortState = GPIO_PIN_RESET;
  
  return FUNC_OK;  
}

/*Установка значений всех выходов GPIO*/
uint32_t DevicePCA9554SetAllOutputPorts(DevicePCA9554* pDev, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode || !pPortState)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE; 
  
  /*Установка значений всех выходов GPIO*/
  uint32_t Res;
  uint8_t OutputByte = 0x0;
  for(uint8_t i = 0; i < PCA9554_NUM_GPIO; i++)
  {
    if(pPortState[i] == GPIO_PIN_SET)
      OutputByte |= PORT_PIN(i);
  }
  
  Res = prvDevicePCA9554WriteRegister(pDev, CMD_OUTPUT_PORT_REG, OutputByte, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  pDev->OutputPortRegister = OutputByte;
  
  return FUNC_OK;  
}

/*Установка значения отдельного выхода GPIO*/
uint32_t DevicePCA9554SetOutputPort(DevicePCA9554* pDev, uint8_t NumPort, GPIO_PinState PortState, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;  
  
  if(NumPort > PCA9554_NUM_GPIO - 1)
    return FUNC_INVALID_PARAM;
  
  if(((PORT_PIN(NumPort) & pDev->ConfRegister)>>NumPort) == PORT_PIN_INPUT)
    return FUNC_INVALID_PARAM;
  
  uint8_t GPIOPort;
  uint32_t Res;
  
  /*Установка значения выхода GPIO*/
  if(PortState == GPIO_PIN_SET)
    GPIOPort = 1;
  else
    GPIOPort = 0;
  
  uint8_t OutputByte = pDev->OutputPortRegister;
  SET_BIT_OF_BYTE(OutputByte, GPIOPort, NumPort);
    
  Res = prvDevicePCA9554WriteRegister(pDev, CMD_OUTPUT_PORT_REG, OutputByte, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  pDev->OutputPortRegister = OutputByte;
  
  return FUNC_OK;
}

/**
@}
*/

/*Чтение регистра портов GPIO*/
static uint32_t prvDevicePCA9554ReadRegister(DevicePCA9554* pDev, uint8_t* pInputPortReg, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode || !pInputPortReg)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;
  
  HAL_StatusTypeDef HALRes;
  uint8_t WriteData;
  
  /*Передача команды*/
  WriteData = (uint8_t)CMD_INPUT_PORT_REG;
  
  HALRes = HAL_I2C_Master_Transmit(&pDev->I2CDrv, pDev->DevAddress, &WriteData, 1, OS_TIME_WAIT_I2C);
  
  if(HALRes != HAL_OK)  
  {
    *pExCode = DEVICE_PCA9554_ERROR_PROCESS_I2C;
    return FUNC_ERROR;
  }  
  
  /*Чтение данных*/
  uint8_t ReadData = 0xFF;
  
  HALRes = HAL_I2C_Master_Receive(&pDev->I2CDrv, pDev->DevAddress, &ReadData, 1, OS_TIME_WAIT_I2C);
   
  if(HALRes != HAL_OK)  
  {
    *pExCode = DEVICE_PCA9554_ERROR_WRITE_I2C;
    return FUNC_ERROR;
  }    
  
  *pInputPortReg = ReadData;
  return FUNC_OK;
}

/*Запись в регистр PCA9554*/
static uint32_t prvDevicePCA9554WriteRegister(DevicePCA9554* pDev, DevicePCA9554TypeCmd TypeCmd, uint8_t ConfByte, ExtCodeDevicePCA9554* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode = DEVICE_PCA9554_NOT_CODE;
  
  if(TypeCmd == CMD_INPUT_PORT_REG)
    return FUNC_INVALID_PARAM;
  
  HAL_StatusTypeDef HALRes;
  uint8_t WriteData[LENGTH_PACKET];
  
  /*Передача данных*/
  WriteData[0] = (uint8_t)TypeCmd;
  WriteData[1] = ConfByte;
  
  HALRes = HAL_I2C_Master_Transmit(&pDev->I2CDrv, pDev->DevAddress, WriteData, LENGTH_PACKET, OS_TIME_WAIT_I2C);
  
  if(HALRes != HAL_OK)  
  {
    *pExCode = DEVICE_PCA9554_ERROR_PROCESS_I2C;
    return FUNC_ERROR;
  }
  
  /*Верификация*/  
  uint8_t ReadData = 0xFF;
  
  HALRes = HAL_I2C_Master_Receive(&pDev->I2CDrv, pDev->DevAddress, &ReadData, 1, OS_TIME_WAIT_I2C);
   
  if(HALRes != HAL_OK)  
  {
    *pExCode = DEVICE_PCA9554_ERROR_WRITE_I2C;
    return FUNC_ERROR;
  }  
  
  if(WriteData[1] != ReadData)
  {
    *pExCode = DEVICE_PCA9554_ERROR_WRITE_I2C;
    return FUNC_ERROR;    
  }
  
  return FUNC_OK;
}