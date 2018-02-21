/**
  \file    DeviceCC1200.h
  \brief   Заголовочный файл драйвера приёмопередатчика CC1200
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __DEVICE_CC1200__
#define __DEVICE_CC1200__

#include "TypesDefine.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"

/**
@}
  \addtogroup module_CC1200
@{
*/

///Время ожидания приёма данных не ограничено
#define CC1200_RECEIVE_WAIT_FOREVER     0                       

///Режимы работы приёмопередатчика
typedef enum _DeviceCC1200Mode {
  DEVICE_CC1200_IDLE = 0x0,                     ///<Ожидание
  DEVICE_CC1200_SLEEP,                          ///<Сон
  DEVICE_CC1200_RECEIVE,                        ///<Приём
  DEVICE_CC1200_TRANSMIT                        ///<Передача
} DeviceCC1200Mode;

///Структура драйвера приёмопередатчика
typedef struct _DeviceCC1200 {
  SPI_HandleTypeDef SPIDrv;                     ///<Указатель на структуру драйвера SPI
  DMA_HandleTypeDef  DMATXDrv;                  ///<Указатель на структуру драйвера SPI DMA TX
  DMA_HandleTypeDef  DMARXDrv;                  ///<Указатель на структуру драйвера SPI DMA RX
  DeviceCC1200Mode   DevMode;                   ///<Режим работы приёмопередатчика
  uint8_t            DevAddr;                   ///<Адрес устройства в беспроводной сети
  
  osSemaphoreId      SemIDSyncDMA;              ///<Семафор ожидания окончания приёма/передачи данных по DMA
} DeviceCC1200;

///Структура параметров инициализации драйвера приёмопередатчика
typedef struct _DeviceCC1200Param {
  SPI_TypeDef*          pInstanceSPIDrv;        ///<Указатель на параметры драйвера SPI
  uint32_t              BaudRateSPIPrescaler;   ///<Скорость работы SPI 
  DMA_Stream_TypeDef*   pInstanceDMATXDrv;      ///<Указатель на параметры драйвера SPI DMA TX
  DMA_Stream_TypeDef*   pInstanceDMARXDrv;      ///<Указатель на параметры драйвера SPI DMA RX
  uint32_t              DMATXChannel;           ///<Номер канала SPI DMA TX
  uint32_t              DMARXChannel;           ///<Номер канала SPI DMA RX
  uint8_t               DevAddr;                ///<Адрес устройства в беспроводной сети
} DeviceCC1200Param;

///Специальные коды ошибок
typedef enum _ExtCodeDeviceCC1200 {
  DEVICE_CC1200_NOT_CODE = 0,                   ///<Ошибок нет
  DEVICE_CC1200_ERROR_TIMEOUT,                  ///<Время ожидания истекло
  DEVICE_CC1200_ERROR_INIT_SPI,                 ///<Ошибка инициализации SPI
  DEVICE_CC1200_ERROR_INIT_DMA,                 ///<Ошибка инициализации DMA
  DEVICE_CC1200_ERROR_PROCESS_DMA,              ///<Ошибка при работе с DMA
  DEVICE_CC1200_ERROR_WRITE_SPI,                ///<Ошибка записи данных по SPI
  DEVICE_CC1200_ERROR_RX_BUF_TIMEOUT,           ///<Время ожидания приёма данных истекло
  DEVICE_CC1200_ERROR_RX_BUF_UNDERFLOW,         ///<Буфер RX пустой
  DEVICE_CC1200_ERROR_RX_BUF_OVERFLOW,          ///<Буфер RX переполнен
  DEVICE_CC1200_ERROR_RX_BUF_ERROR,             ///<Ошибка при работе с буфером RX
  DEVICE_CC1200_ERROR_TX_BUF_UNDERFLOW,         ///<Буфер TX пустой
  DEVICE_CC1200_ERROR_TX_BUF_OVERFLOW,          ///<Буфер TX переполнен
  DEVICE_CC1200_ERROR_TX_BUF_ERROR,             ///<Ошибка при работе с буфером TX
  DEVICE_CC1200_ERROR_RECEIVE                   ///<Ошибка приёма данных
} ExtCodeDeviceCC1200;

/**Инициализация драйвера приёмопередатчика
  \param[in] pDevParam указатель на структуру параметров для инициализации драйвера
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceCC1200Create(DeviceCC1200Param* pDevParam, DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode);

/**Приём пакета данных фиксированного размера
  \param[in] pDev указатель на структуру драйвера
  \param[in] FixNumPacket размер пакета данных
  \param[out] pPacket принимаемый пакет данных
  \param[out] pExCode специальный код ошибки выполнения функции
  \param[in] Waiting время ожидания прёма данных
  \return Результат выполнения функции 
*/ 
uint32_t DeviceCC1200ReceiveFixPacket(DeviceCC1200* pDev, uint8_t FixNumPacket, uint8_t* pPacket, ExtCodeDeviceCC1200* pExCode, uint32_t Waiting);

/**Передача пакета данных фиксированного размера
  \param[in] pDev указатель на структуру драйвера
  \param[in] FixNumPacket размер пакета данных
  \param[out] pPacket передаваемый пакет данных
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceCC1200TransmitFixPacket(DeviceCC1200* pDev, uint8_t FixNumPacket, uint8_t* pPacket, ExtCodeDeviceCC1200* pExCode);

/**Перезагрузка приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции  
  \return Результат выполнения функции 
*/
uint32_t DeviceCC1200Reset(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode);

/**Чтение состояния приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[out] pChipStatusByte состояние приёмопередатчика
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
uint32_t DeviceCC1200ReadChipStatus(DeviceCC1200* pDev, uint8_t* pChipStatusByte, ExtCodeDeviceCC1200* pExCode);

/**
@}
*/

#endif