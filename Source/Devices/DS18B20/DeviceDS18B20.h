/**
  \file    DeviceDS18B20.h
  \brief   Заголовочный файл драйвера датчика температуры DS18B20
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_DS18B20
#define __DEVICE_DS18B20

#include "stm32f0xx_hal.h"
#include "TypesDefine.h"

/**
  \addtogroup module_DS18B20
@{
*/

/** @name Команды для датчика температуры
*/                        
///@{
#define  SEARCH_ROM          0xF0               ///<Поиск микросхемы по ROM коду
#define  READ_ROM            0x33               ///<Чтение ROM кода микросхемы
#define  MATCH_ROM           0x55               ///<Адресация к конкретной микросхеме по ROM коду
#define  SKIP_ROM            0xCC               ///<Обращение к микросхеме без ROM кода
#define  ALARM_SEARCH        0xEC               ///<Индикация ошибки микросхем
#define  CONVERT_T           0x44               ///<Конвертация данных о температуре
#define  READ_SCRATCHPAD     0xBE               ///<Чтение содержимого памяти микросхемы
#define  WRITE_SCRATCHPAD    0x4E               ///<Запись в память микросхемы
#define  COPY_SCRATCHPAD     0x48               ///<Копирование данных памяти микросхемы
#define  RECALL_E2           0xB8               ///<Чтение статуса состояния пороговых значений температуры
#define  READ_POWER_SUPPLY   0xB4               ///<Чтение режима питания микросхемы
///@}

/** @name Флаги готовности выдачи данных температуры
*/                        
///@{
#define TEMP_NOT_READY       0x0                ///<Данные не готовы
#define TEMP_READY           0x1                ///<Данные готовы
///@}

///Структура драйвера датчика температуры
typedef struct _DeviceDS18B20 {
  GPIO_TypeDef*     pGPIOTXPort;                ///<Указатель на структуру GPIO для сигнала TX
  GPIO_TypeDef*     pGPIORXPort;                ///<Указатель на структуру GPIO для сигнала RX
  uint16_t          GPIOTXNum;                  ///<Номер GPIO для сигнала TX
  uint16_t          GPIORXNum;                  ///<Номер GPIO для сигнала RX
} DeviceDS18B20;

///Структура параметров инициализации датчика температуры
typedef struct _DeviceDS18B20Param {    
  char*             GPIOTXPin;                  ///<Строка описания номера пина микроконтроллера для сигнала TX
  char*             GPIORXPin;                  ///<Строка описания номера пина микроконтроллера для сигнала RX
} DeviceDS18B20Param;

///Специальные коды ошибок
typedef enum _ExtCodeDeviceDS18B20 {
  DEVICE_DS18B20_NOT_CODE = 0,                  ///<Ошибок нет
  DEVICE_DS18B20_ERROR_INIT_GPIO,               ///<Ошибка инициализации GPIO
  DEVICE_DS18B20_ERROR_NOT_RESP,                ///<Датчик температуры не отвечает
  DEVICE_DS18B20_ERROR_CRC                      ///<Ошибка целостности данных
} ExtCodeDeviceDS18B20;

/**Инициализация драйвера датчика температуры
  \param[in] pDevParam указатель на структуру параметров для инициализации драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceDS18B20Create(DeviceDS18B20Param* pDevParam, ExtCodeDeviceDS18B20* pExCode);

/**Запрос состояния готовности выдачи данных о температуре
  \param[out] pTempReady флаг готовности
  \return Результат выполнения функции 
*/ 
uint32_t DeviceDS18B20GetReadyTemperature(uint8_t* pTempReady);

/**Запуск процесса конвертации данных о температуре
  \return Результат выполнения функции 
*/ 
uint32_t DeviceDS18B20ConversionTemperature();

/**Чтение температуры с датчика
  \param[out] pTemp температура в градусах Цельсия
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceDS18B20GetTemperature(int8_t* pTemp, ExtCodeDeviceDS18B20* pExCode);

/**
@}
*/

#endif