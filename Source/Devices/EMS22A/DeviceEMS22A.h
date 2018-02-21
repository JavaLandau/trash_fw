/**
  \file    DeviceEMS22A.h 
  \brief   Заголовочный файл драйвера абсолютного энкодера EMS22A
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_EMS22A__
#define __DEVICE_EMS22A__

#include "TypesDefine.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/**
  \addtogroup module_EMS22A
@{
*/

///Структура драйвера энкодера
typedef struct _DeviceEMS22A {
  SPI_HandleTypeDef SPIDrv;                     ///<Указатель на структуру драйвера SPI
} DeviceEMS22A;

///Структура параметров инициализации драйвера энкодера
typedef struct _DeviceEMS22AParam {
  SPI_TypeDef*          pInstanceSPIDrv;        ///<Указатель на параметры драйвера SPI          
  uint32_t              BaudRateSPIPrescaler;   ///<Скорость работы SPI 
} DeviceEMS22AParam;

///Специальные коды ошибок
typedef enum _ExtCodeDeviceEMS22A {
  DEVICE_EMS22A_NOT_CODE = 0,                   ///<Ошибок нет
  DEVICE_EMS22A_ERROR_INIT_SPI,                 ///<Ошибка инициализации SPI
  DEVICE_EMS22A_ERROR_WRITE_SPI,                ///<Ошибка записи данных по SPI
  DEVICE_EMS22A_ERROR_PROCESS_SPI,              ///<Ошибка в процессе работы с SPI
  DEVICE_EMS22A_ERROR_CRC                       ///<Ошибка целостности данных
} ExtCodeDeviceEMS22A;

/**Инициализация драйвера энкодера
  \param[in] pDevParam указатель на структуру параметров для инициализации драйвера
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceEMS22ACreate(DeviceEMS22AParam* pDevParam, DeviceEMS22A* pDev, ExtCodeDeviceEMS22A* pExCode);

/**Чтение значения угла поворота
  \param[in] pDev указатель на структуру драйвера
  \param[in] pAng угол поворота
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceEMS22AGetAngular(DeviceEMS22A* pDev, float* pAng, ExtCodeDeviceEMS22A* pExCode);
    
/**Чтение данных с энкодера
  \param[in] pDev указатель на структуру драйвера
  \param[in] pCode считываемые данные
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceEMS22AGetCode(DeviceEMS22A* pDev, uint16_t* pCode, ExtCodeDeviceEMS22A* pExCode);

/**
@}
*/

#endif