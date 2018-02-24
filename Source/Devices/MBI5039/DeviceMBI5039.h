/**
  \file    DeviceMBI5039.h 
  \brief   Заголовочный файл драйвера светодиодов MBI5039
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_MBI5039
#define __DEVICE_MBI5039

#include "stm32f0xx_hal.h"
#include "TypesDefine.h"

/**
  \addtogroup module_MBI5039
@{
*/

///Структура драйвера светодиодов
typedef struct _DeviceMBI5039 {
  GPIO_TypeDef*     pGPIONSSPort;               ///<Указатель на структуру GPIO для сигнала NSS
  uint16_t          GPIONSSNum;                 ///<Номер GPIO для сигнала NSS
  SPI_HandleTypeDef SPIDrv;                     ///<Указатель на структуру драйвера SPI
  uint8_t           devNum;                     ///<Число последовательно подключенных микросхем
} DeviceMBI5039;

///Структура параметров инициализации драйвера светодиодов
typedef struct _DeviceMBI5039Param {    
  SPI_TypeDef*      pInstanceSPIDrv;            ///<Указатель на параметры драйвера SPI          
  uint32_t          BaudRateSPIPrescaler;       ///<Скорость работы SPI 
  char*             GPIONSSPin;                 ///<Строка описания номера пина микроконтроллера для сигнала NSS
  uint8_t           devNum;                     ///<Число последовательно подключенных микросхем
} DeviceMBI5039Param;

///Специальные коды ошибок
typedef enum _ExtCodeDeviceMBI5039 {
  DEVICE_MBI5039_NOT_CODE = 0,                  ///<Ошибок нет
  DEVICE_MBI5039_ERROR_INIT_SPI,                ///<Ошибка инициализации SPI
  DEVICE_MBI5039_ERROR_INIT_GPIO,               ///<Ошибка инициализации GPIO
  DEVICE_MBI5039_ERROR_PROCESS_SPI,             ///<Ошибка в процессе работы с SPI
  DEVICE_MBI5039_ERROR_WRITE_SPI                ///<Ошибка записи данных по SPI
} ExtCodeDeviceMBI5039;

/**Инициализация драйвера светодиодов
  \param[in] pDevParam указатель на структуру параметров для инициализации драйвера
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceMBI5039Create(DeviceMBI5039Param* pDevParam, DeviceMBI5039* pDev, ExtCodeDeviceMBI5039* pExCode);

/**Установка выходов драйвера 
  \param[in] pDev указатель на структуру драйвера
  \param[in] pOutput значения выходов драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceMBI5039Set(DeviceMBI5039* pDev, uint16_t* pOutput, ExtCodeDeviceMBI5039* pExCode);

/**Сброс выходов драйвера 
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceMBI5039Reset(DeviceMBI5039* pDev,  ExtCodeDeviceMBI5039* pExCode);

/**
@}
*/

#endif