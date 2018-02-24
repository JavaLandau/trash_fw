/**
  \file    DeviceAD5370.h 
  \brief   Заголовочный файл драйвера ЦАП AD5370
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_AD5370
#define __DEVICE_AD5370

#include "stm32f0xx_hal.h"
#include "cmsis_os.h"

/**
  \addtogroup module_AD5370
@{
*/

#define NUM_CHANNELS            40              ///<Общее количество каналов

#define SELECT_A                0x0             ///<Выбор регистра X2A для каналов ЦАП
#define SELECT_B                0x4             ///<Выбор регистра X2B для каналов ЦАП

#define TEMP_SHUTDOWN_ENABLE    0x2             ///<Разрешить отключение устройства при перегреве
#define TEMP_SHUTDOWN_DISABLE   0x0             ///<Запретить отключение устройства при перегреве

#define SOFT_POWER_DOWN         0x1             ///<ЦАП отключен
#define SOFT_POWER_UP           0x0             ///<ЦАП включен

///Опорное напряжение для ЦАП
typedef enum _DeviceAD5370Vref {
  VREF_3V = 0x0,                                ///<Опорное напряжение 3 В
  VREF_5V                                       ///<Опорное напряжение 5 В
} DeviceAD5370Vref;

///Структура драйвера ЦАП
typedef struct _DeviceAD5370 {
  SPI_HandleTypeDef SPIDrv;                    ///<Указатель на структуру драйвера SPI
  
  GPIO_TypeDef*     pGPIOSyncPort;             ///<Указатель на структуру GPIO для сигнала \f$\overline{SYNC}\f$ 
  GPIO_TypeDef*     pGPIOResetPort;            ///<Указатель на структуру GPIO для сигнала \f$\overline{RESET}\f$  
  GPIO_TypeDef*     pGPIOBusyPort;             ///<Указатель на структуру GPIO для сигнала \f$\overline{BUSY}\f$ 
  GPIO_TypeDef*     pGPIOLDACPort;             ///<Указатель на структуру GPIO для сигнала \f$\overline{LDAC}\f$  
  GPIO_TypeDef*     pGPIOCLRPort;              ///<Указатель на структуру GPIO для сигнала \f$\overline{CLR}\f$    
  
  uint16_t          GPIOSyncNum;               ///<Номер GPIO для сигнала \f$\overline{SYNC}\f$  
  uint16_t          GPIOResetNum;              ///<Номер GPIO для сигнала \f$\overline{RESET}\f$   
  uint16_t          GPIOBusyNum;               ///<Номер GPIO для сигнала \f$\overline{BUSY}\f$   
  uint16_t          GPIOLDACNum;               ///<Номер GPIO для сигнала \f$\overline{LDAC}\f$   
  uint16_t          GPIOCLRNum;                ///<Номер GPIO для сигнала \f$\overline{CLR}\f$   
} DeviceAD5370;

///Структура параметров инициализации драйвера ЦАП
typedef struct _DeviceAD5370Param {
  DeviceAD5370Vref  Vref;                      ///<Опорное напряжение ЦАП 
  SPI_TypeDef*      pInstanceSPIDrv;           ///<Указатель на параметры драйвера SPI          
  uint32_t          BaudRateSPIPrescaler;      ///<Скорость работы SPI 
  
  char*             GPIOSyncPin;               ///<Строка описания номера пина микроконтроллера для сигнала \f$\overline{SYNC}\f$  
  char*             GPIOResetPin;              ///<Строка описания номера пина микроконтроллера для сигнала \f$\overline{RESET}\f$   
  char*             GPIOBusyPin;               ///<Строка описания номера пина микроконтроллера для сигнала \f$\overline{BUSY}\f$   
  char*             GPIOLDACPin;               ///<Строка описания номера пина микроконтроллера для сигнала \f$\overline{LDAC}\f$   
  char*             GPIOCLRPin;                ///<Строка описания номера пина микроконтроллера для сигнала \f$\overline{CLR}\f$   
} DeviceAD5370Param;

///Специальные коды ошибок
typedef enum _ExtCodeDeviceAD5370 {
  DEVICE_AD5370_NOT_CODE = 0,                   ///<Ошибок нет
  DEVICE_AD5370_ERROR_INIT_SPI,                 ///<Ошибка инициализации SPI
  DEVICE_AD5370_ERROR_INIT_GPIO,                ///<Ошибка инициализации GPIO
  DEVICE_AD5370_ERROR_PROCESS_SPI,              ///<Ошибка в процессе работы с SPI
  DEVICE_AD5370_ERROR_WRITE_SPI                 ///<Ошибка записи данных в ЦАП по SPI
} ExtCodeDeviceAD5370;

/**Инициализация драйвера ЦАП
  \param[in] pDevParam указатель на структуру параметров для инициализации драйвера
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceAD5370Create(DeviceAD5370Param* pDevParam, DeviceAD5370* pDev, ExtCodeDeviceAD5370* pExCode);

/**Установка всех каналов ЦАП
  \param[in] pDev указатель на структуру драйвера
  \param[in] pOutput значения для всех каналов ЦАП
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceAD5370SetAllChannels(DeviceAD5370* pDev, uint16_t* pOutput, ExtCodeDeviceAD5370* pExCode);

/**Перезагрузка ЦАП
  \param[in] pDev указатель на структуру драйвера
  \return Результат выполнения функции 
*/
uint32_t DeviceAD5370Reset(DeviceAD5370* pDev);

/**Установка заданного канала ЦАП
  \param[in] pDev указатель на структуру драйвера
  \param[in] ValueChannel значение канала
  \param[in] NumChannel номер канала
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DeviceAD5370SetOneChannel(DeviceAD5370* pDev, uint16_t ValueChannel, uint8_t NumChannel, ExtCodeDeviceAD5370* pExCode);

/**
@}
*/

#endif