/**
  \file    DeviceCC1200.c 
  \brief   Исполняемый файл драйвера приёмопередатчика CC1200
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include <stdlib.h>
#include "DeviceCC1200.h"
#include "DeviceCC1200Defs.h"
//#include "TCPCommandConsole.h"

/**
  \defgroup module_service_CC1200 Служебные функции для работы с CC1200
  \brief Модуль служебных функций, необходимых для работы с микросхемой приёмопередатчика CC1200
@{
*/

#define TIME_WAIT_SEM_SPI_DMA   1000                    ///<Время ожидания окончания приема/передачи данных SPI по DMA

#define __STATE(BYTE)           (((BYTE)>>0x4)&0x7)     ///<Текущее состояние устройства
#define __CHIP_RDYn(BYTE)       (((BYTE)>>0x7)&0x1)     ///<Готовность устройства

#define WRITE_SINGLE_ACCESS            0x00             ///<Режим записи байта
#define READ_SINGLE_ACCESS             0x80             ///<Режим чтения байта

#define WRITE_BURST_ACCESS             0x40             ///<Пакетный режим записи данных
#define READ_BURST_ACCESS              0xC0             ///<Пакетный режим чтения данных

#define RX_FIFO_BUF_SIZE               0x80             ///<Размер буфера принимаемых данных в байтах

#define CC1200_TIMEOUT_INFINITE        0                ///<Время ожидания не ограничено
#define TIME_WAITING_RECEIVE           1                ///<Время ожидания перед приёмом данных

/**Установка команды для приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[in] AddressCmd код команды
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/ 
static uint32_t prvDeviceCC1200CommandStrobes(DeviceCC1200* pDev, uint8_t AddressCmd, ExtCodeDeviceCC1200* pExCode);

/**Чтение регистра из расширенной области памяти приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[in] AddrReg адрес регистра
  \param[out] pValueReg значение считываемого регистра
  \param[out] pStatusByte значение считываемого байта состояния устройства
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200ReadExtendedRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t* pValueReg, uint8_t* pStatusByte, ExtCodeDeviceCC1200* pExCode);

/**Чтение регистра из стандартной области памяти приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[in] AddrReg адрес регистра
  \param[out] pValueReg значение считываемого регистра
  \param[out] pStatusByte значение считываемого байта состояния устройства
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200ReadRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t* pValueReg, uint8_t* pStatusByte, ExtCodeDeviceCC1200* pExCode);

/**Запись регистра из расширенной области памяти приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[in] AddrReg адрес регистра
  \param[in] ValueReg значение, записываемое в регистр
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200WriteExtendedRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t ValueReg, ExtCodeDeviceCC1200* pExCode);

/**Запись регистра из стандартной области памяти приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[in] AddrReg адрес регистра
  \param[in] ValueReg значение, записываемое в регистр
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200WriteRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t ValueReg, ExtCodeDeviceCC1200* pExCode);

/**Установка параметров приёмопередатчика по умолчанию
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200SetDefsParams(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode);

/**Установка режима приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[in] DevMode режим приёмопередатчика
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200SetMode(DeviceCC1200* pDev, DeviceCC1200Mode DevMode, ExtCodeDeviceCC1200* pExCode);

/**Чтение байта из буфера RX
  \param[in] pDev указатель на структуру драйвера
  \param[out] pValue считываемый байт
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200PopRXFIFO(DeviceCC1200* pDev, uint8_t *pValue, ExtCodeDeviceCC1200* pExCode);

/**Запись байта в буфер TX
  \param[in] pDev указатель на структуру драйвера
  \param[in] Value записываемый байт
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200PushTXFIFO(DeviceCC1200* pDev, uint8_t Value, ExtCodeDeviceCC1200* pExCode);

/**Очистка буфера RX
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200FlushRXFIFO(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode);

/**Функция ожидания готовности приёмопередатчика
  \param[in] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \param[in] Timeout время ожидания
  \return Результат выполнения функции 
*/
static uint32_t prvDeviceCC1200WaitStausByte(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode, uint32_t Timeout);

/**
@}
  \defgroup module_CC1200 Интерфейсные функции для работы с CC1200
  \brief Модуль, предоставляющий пользователю необходимый функционал для работы с микросхемой приёмопередатчика CC1200
@{
*/

/*Инициализация драйвера приёмопередатчика*/
uint32_t DeviceCC1200Create(DeviceCC1200Param* pDevParam, DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных данных*/
  if(!pExCode)
    return FUNC_INVALID_PARAM;
    
  *pExCode = DEVICE_CC1200_NOT_CODE;
  
  if(!pDevParam || !pDev)
    return FUNC_INVALID_PARAM;
    
    
  pDev->DevAddr = pDevParam->DevAddr;
  
  /*Инициализация SPI*/
  pDev->SPIDrv.State = HAL_SPI_STATE_RESET;
  pDev->SPIDrv.Instance = pDevParam->pInstanceSPIDrv;
  pDev->SPIDrv.Init.Mode = SPI_MODE_MASTER;
  pDev->SPIDrv.Init.Direction = SPI_DIRECTION_2LINES;
  pDev->SPIDrv.Init.DataSize = SPI_DATASIZE_8BIT;
  pDev->SPIDrv.Init.CLKPolarity = SPI_POLARITY_LOW;
  pDev->SPIDrv.Init.CLKPhase = SPI_PHASE_1EDGE;
  pDev->SPIDrv.Init.NSS = SPI_NSS_HARD_OUTPUT;
  pDev->SPIDrv.Init.BaudRatePrescaler = pDevParam->BaudRateSPIPrescaler;
  pDev->SPIDrv.Init.FirstBit = SPI_FIRSTBIT_MSB;
  pDev->SPIDrv.Init.TIMode = SPI_TIMODE_DISABLE;
  pDev->SPIDrv.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  pDev->SPIDrv.Init.CRCPolynomial = 10;
  pDev->SPIDrv.pUserData = (void*)*ppDev;
  if (HAL_SPI_Init(&pDev->SPIDrv) != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_INIT_SPI;
    return FUNC_ERROR;
  }
 
  /*Инициализация DMA TX*/
  pDev->DMATXDrv.Instance = pDevParam->pInstanceDMATXDrv;
  pDev->DMATXDrv.Init.Channel = pDevParam->DMATXChannel;
  pDev->DMATXDrv.Init.Direction = DMA_MEMORY_TO_PERIPH;
  pDev->DMATXDrv.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  pDev->DMATXDrv.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  pDev->DMATXDrv.Init.MemBurst = DMA_MBURST_SINGLE;
  pDev->DMATXDrv.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  pDev->DMATXDrv.Init.MemInc = DMA_MINC_ENABLE;
  pDev->DMATXDrv.Init.Mode = DMA_NORMAL;
  pDev->DMATXDrv.Init.PeriphBurst = DMA_PBURST_SINGLE;
  pDev->DMATXDrv.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  pDev->DMATXDrv.Init.PeriphInc = DMA_PINC_DISABLE;
  pDev->DMATXDrv.Init.Priority = DMA_PRIORITY_MEDIUM;  
  if (HAL_DMA_Init(&(pDev->DMATXDrv)) != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_INIT_DMA;
    return FUNC_ERROR;
  }
  pDev->DMATXDrv.Parent = (void*)(&pDev->SPIDrv);
   
  /*Инициализация DMA RX*/
  pDev->DMARXDrv.Instance = pDevParam->pInstanceDMARXDrv;
  pDev->DMARXDrv.Init.Channel = pDevParam->DMARXChannel;
  pDev->DMARXDrv.Init.Direction = DMA_PERIPH_TO_MEMORY;
  pDev->DMARXDrv.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  pDev->DMARXDrv.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  pDev->DMARXDrv.Init.MemBurst = DMA_MBURST_SINGLE;
  pDev->DMARXDrv.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  pDev->DMARXDrv.Init.MemInc = DMA_MINC_ENABLE;
  pDev->DMARXDrv.Init.Mode = DMA_NORMAL;
  pDev->DMARXDrv.Init.PeriphBurst = DMA_PBURST_SINGLE;
  pDev->DMARXDrv.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  pDev->DMARXDrv.Init.PeriphInc = DMA_PINC_DISABLE;
  pDev->DMARXDrv.Init.Priority = DMA_PRIORITY_MEDIUM;
  if (HAL_DMA_Init(&(pDev->DMARXDrv)) != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_INIT_DMA;
    return FUNC_ERROR;
  }  
  pDev->DMARXDrv.Parent = (void*)(&pDev->SPIDrv);
  
  pDev->SPIDrv.hdmatx = &(pDev->DMATXDrv);
  pDev->SPIDrv.hdmarx = &(pDev->DMARXDrv);

  pDev->SemIDSyncDMA = osSemaphoreCreate(NULL, 1);
  osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  uint32_t Res;
  
  /*Перезагрузка приёмопередатчика*/
  Res = DeviceCC1200Reset(*ppDev, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WaitStausByte(*ppDev, pExCode, CC1200_TIMEOUT_INFINITE);
  
  if(Res != FUNC_OK)
    return Res;    
  
  /*Установка параметров по умолчанию*/
  Res = prvDeviceCC1200SetDefsParams(*ppDev, pExCode);
    
  return Res;  
}

/**
@}
  \addtogroup module_service_CC1200
@{
*/

/** @name Определения для переконфигурации регистров приёмопередатчика на передачу
*/                        
///@{
#define DEF_FS_DIG1_TX                  0x04
#define DEF_FS_DIG0_TX                  0x50
#define DEF_FS_DVC1_TX                  0xF7
#define DEF_FS_DVC0_TX                  0x0F
///@}

/**
@}
  \addtogroup module_CC1200
@{
*/

/*Передача пакета данных фиксированного размера*/
uint32_t DeviceCC1200TransmitFixPacket(DeviceCC1200* pDev, uint8_t FixNumPacket, uint8_t* pPacket, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных данных*/
  if(!pDev || !pPacket || !pExCode || !FixNumPacket)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  uint32_t Res;
  
  /*Переконфигурация регистров на передачу*/
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DIG1, DEF_FS_DIG1_TX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;       
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DIG0, DEF_FS_DIG0_TX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;      
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DVC1, DEF_FS_DVC1_TX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DVC0, DEF_FS_DVC0_TX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;    
  
  /*Получение количества байт, которые еще не переданы*/
  uint8_t TXBytes;    
  Res = prvDeviceCC1200ReadExtendedRegister(pDev, NUM_TXBYTES, &TXBytes, NULL, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  /*Если в буфере есть данные, возвращаем ошибку*/
  if(TXBytes)  
  {
    *pExCode = DEVICE_CC1200_ERROR_TX_BUF_OVERFLOW;
    return FUNC_ERROR;
  }  
  
  /*Отправка пакета данных через однобайтный режим передачи*/
  Res = prvDeviceCC1200PushTXFIFO(pDev, FixNumPacket + 1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200PushTXFIFO(pDev, pDev->DevAddr, pExCode);
  
  if(Res != FUNC_OK)
    return Res;   
  
  for(uint8_t i = 0; i < FixNumPacket; i++)  
  {
    Res = prvDeviceCC1200PushTXFIFO(pDev, pPacket[i], pExCode);
    
    if(Res != FUNC_OK)
      return Res;
  }  
  
  /*Проверка количества переданных в буфер TX байт*/
  Res = prvDeviceCC1200ReadExtendedRegister(pDev, NUM_TXBYTES, &TXBytes, NULL, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  if(TXBytes != FixNumPacket + 2)  
  {
    *pExCode = DEVICE_CC1200_ERROR_TX_BUF_ERROR;
    return FUNC_ERROR;
  }    
  
  /*Установка режима устройства на передачу*/
  Res = prvDeviceCC1200SetMode(pDev, DEVICE_CC1200_TRANSMIT, pExCode);
    
  /*Ожидание передачи пакета*/
  while(TXBytes)
  {
    Res = prvDeviceCC1200ReadExtendedRegister(pDev, NUM_TXBYTES, &TXBytes, NULL, pExCode);
    
    if(Res != FUNC_OK)
      return Res;    
  }
  
  /*Ожидание перехода состояния устройства в режим ожидания*/  
  uint8_t ChipStatusByte;
    
  do 
  {
    Res = DeviceCC1200ReadChipStatus(pDev, &ChipStatusByte, pExCode);
    
    if(Res != FUNC_OK)
      return Res;       
  } while(__STATE(ChipStatusByte) != CHIP_STATUS_IDLE);  
        
  return FUNC_OK;  
}

/**
@}
  \addtogroup module_service_CC1200
@{
*/

/** @name Определения для переконфигурации регистров приёмопередатчика на приём
*/                        
///@{
#define DEF_FS_DIG1_RX                  0x07
#define DEF_FS_DIG0_RX                  0xAB
#define DEF_FS_DVC1_RX                  0xFF
#define DEF_FS_DVC0_RX                  0x17
///@}

/**
@}
  \addtogroup module_CC1200
@{
*/

/*Приём пакета данных фиксированного размера*/
uint32_t DeviceCC1200ReceiveFixPacket(DeviceCC1200* pDev, uint8_t FixNumPacket, uint8_t* pPacket, ExtCodeDeviceCC1200* pExCode, uint32_t Waiting)
{
  /*Проверка входных данных*/
  if(!pDev || !pPacket || !pExCode || !FixNumPacket)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  uint32_t Res;
  uint8_t RXBytes;
  uint32_t TickWait = 0;
  
  /*Переконфигурация регистров*/  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DIG1, DEF_FS_DIG1_RX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;       
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DIG0, DEF_FS_DIG0_RX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;      
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DVC1, DEF_FS_DVC1_RX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DVC0, DEF_FS_DVC0_RX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;    
  
  /*Установка режима приёма*/
  Res = prvDeviceCC1200SetMode(pDev, DEVICE_CC1200_RECEIVE, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
          
  /*Приём данных*/
  do
  {
    Res = prvDeviceCC1200ReadExtendedRegister(pDev, NUM_RXBYTES, &RXBytes, NULL, pExCode);
    
    if(Res != FUNC_OK)
      return Res;  
    
    if(RXBytes > RX_FIFO_BUF_SIZE)
    {
      //*pExCode  = DEVICE_CC1200_ERROR_RX_BUF_OVERFLOW;        
      Res = prvDeviceCC1200FlushRXFIFO(pDev, pExCode);
      Res = prvDeviceCC1200SetMode(pDev, DEVICE_CC1200_RECEIVE, pExCode);
    }
    
    if(Waiting != CC1200_RECEIVE_WAIT_FOREVER)
    {           
      if(++TickWait == Waiting)
      {
        *pExCode = DEVICE_CC1200_ERROR_RX_BUF_TIMEOUT;
        return FUNC_ERROR;
      }    
      
      osDelay(TIME_WAITING_RECEIVE);
    }
  }while(RXBytes != (FixNumPacket + 2));
  
  /*Проверка заголовка пакета принятых данных*/
  uint8_t LenPacket, DevAddr;
  Res = prvDeviceCC1200PopRXFIFO(pDev, &LenPacket, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
    
  Res = prvDeviceCC1200PopRXFIFO(pDev, &DevAddr, pExCode);
  
  if(Res != FUNC_OK)
    return Res;    
  
  if(LenPacket != (FixNumPacket + 1) || DevAddr != pDev->DevAddr)
  {
    Res = prvDeviceCC1200FlushRXFIFO(pDev, pExCode);
    
    if(Res != FUNC_OK)
      return Res;        
    
    *pExCode = DEVICE_CC1200_ERROR_RECEIVE;
    return FUNC_ERROR;
  }  
  
  /*Чтение пакета принятых данных через однобайтный режим чтения*/
  for(uint8_t i = 0; i < FixNumPacket; i++)  
  {
    Res = prvDeviceCC1200PopRXFIFO(pDev, &pPacket[i], pExCode);
    
    if(Res != FUNC_OK)
      return Res;  
  }
  
  return FUNC_OK;  
}

/*Перезагрузка приёмопередатчика*/
uint32_t DeviceCC1200Reset(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode)
{
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  uint32_t Res;
      
  Res = prvDeviceCC1200CommandStrobes(pDev, SRES, pExCode);
  osDelay(2);
  
  return Res;  
}

/*Чтение состояния приёмопередатчика*/
uint32_t DeviceCC1200ReadChipStatus(DeviceCC1200* pDev, uint8_t* pChipStatusByte, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pChipStatusByte || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE;
  osStatus osRes;
  HAL_StatusTypeDef Res;
  
  /*Чтение состояния приёмопередатчика*/
  uint8_t TransmitData = SNOP | READ_SINGLE_ACCESS;
  
  Res = HAL_SPI_TransmitReceive_DMA(&pDev->SPIDrv, &TransmitData, pChipStatusByte, 1);    
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }
  
  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }
  
  return FUNC_OK;  
}

/**
@}
  \addtogroup module_service_CC1200
@{
*/

/*Чтение регистра из расширенной области памяти приёмопередатчика*/
static uint32_t prvDeviceCC1200ReadExtendedRegister(DeviceCC1200* pDev, uint8_t AddrReg, 
                                                    uint8_t* pValueReg, uint8_t* pStatusByte, 
                                                    ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pValueReg || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  
  /*Чтение данных*/
  uint32_t Res;
  osStatus osRes;
  
  uint8_t TransmitData[3];
  uint8_t ReceiveData[3];
  
  TransmitData[0] = READ_SINGLE_ACCESS | EXTENDED_ADDRESS;
  TransmitData[1] = AddrReg;  
  TransmitData[2] = SNOP;  
  
  Res = HAL_SPI_TransmitReceive_DMA(&pDev->SPIDrv, TransmitData, ReceiveData, 3);
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  

  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }  
  
  *pValueReg = ReceiveData[2];
  
  if(pStatusByte)
    *pStatusByte = ReceiveData[0];  
    
  return FUNC_OK;  
}

/*Чтение байта из буфера RX*/
static uint32_t prvDeviceCC1200PopRXFIFO(DeviceCC1200* pDev, uint8_t *pValue, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pValue || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  
  /*Чтение данных*/
  uint32_t Res;
  osStatus osRes;
  
  uint8_t TransmitData[2];
  uint8_t ReceiveData[2];
  
  TransmitData[0] = READ_SINGLE_ACCESS | STANDART_FIFO_ACCESS;
  TransmitData[1] = SNOP;  
  
  Res = HAL_SPI_TransmitReceive_DMA(&pDev->SPIDrv, TransmitData, ReceiveData, 2);
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  

  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }  
  
  *pValue = ReceiveData[1];
    
  return FUNC_OK;    
}

/*Запись байта в буфер TX*/
static uint32_t prvDeviceCC1200PushTXFIFO(DeviceCC1200* pDev, uint8_t Value, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  
  
  /*Запись данных*/
  uint32_t Res;
  osStatus osRes;
  
  uint8_t TransmitData[2];
  
  TransmitData[0] = WRITE_SINGLE_ACCESS | STANDART_FIFO_ACCESS;
  TransmitData[1] = Value;
  
  Res = HAL_SPI_Transmit_DMA(&pDev->SPIDrv, TransmitData, 2);   
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  
  
  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }    
  
  return FUNC_OK;
}

/*Чтение регистра из стандартной области памяти приёмопередатчика*/
static uint32_t prvDeviceCC1200ReadRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t* pValueReg, 
                                            uint8_t* pStatusByte, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pValueReg || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 

  if(AddrReg >= EXTENDED_ADDRESS)
    return FUNC_INVALID_PARAM;  
  
  /*Чтение данных*/
  uint32_t Res;
  osStatus osRes;
  
  uint8_t TransmitData[2];
  uint8_t ReceiveData[2];
  
  TransmitData[0] = READ_SINGLE_ACCESS | AddrReg;
  TransmitData[1] = SNOP;  
  
  Res = HAL_SPI_TransmitReceive_DMA(&pDev->SPIDrv, TransmitData, ReceiveData, 2);
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  

  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }  
  
  *pValueReg = ReceiveData[1];
  
  if(pStatusByte)
    *pStatusByte = ReceiveData[0];  
    
  return FUNC_OK;  
}

/*Запись регистра из расширенной области памяти приёмопередатчика*/
static uint32_t prvDeviceCC1200WriteExtendedRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t ValueReg, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  
  /*Передача данных*/
  uint32_t Res;
  osStatus osRes;
    
  uint8_t TransmitData[3];
  uint8_t VerifyData;
  
  TransmitData[0] = WRITE_SINGLE_ACCESS | EXTENDED_ADDRESS;
  TransmitData[1] = AddrReg;
  TransmitData[2] = ValueReg;
  
  Res = HAL_SPI_Transmit_DMA(&pDev->SPIDrv, TransmitData, 3);   
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  
  
  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }  
  
  /*Верификация данных*/
  Res = prvDeviceCC1200ReadExtendedRegister(pDev, AddrReg, &VerifyData, NULL, pExCode);
  
  if(Res != FUNC_OK)
    return Res;

  if(VerifyData != ValueReg)  
  {
    *pExCode = DEVICE_CC1200_ERROR_WRITE_SPI;
    return FUNC_ERROR;        
  }
  
  return FUNC_OK;  
}

/*Запись регистра из стандартной области памяти приёмопередатчика*/
static uint32_t prvDeviceCC1200WriteRegister(DeviceCC1200* pDev, uint8_t AddrReg, uint8_t ValueReg, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 

  if(AddrReg >= EXTENDED_ADDRESS)
    return FUNC_INVALID_PARAM;
  
  /*Передача данных*/
  uint32_t Res;
  osStatus osRes;
  
  uint8_t TransmitData[2];
  uint8_t VerifyData;
  
  TransmitData[0] = WRITE_SINGLE_ACCESS | AddrReg;
  TransmitData[1] = ValueReg;
  
  Res = HAL_SPI_Transmit_DMA(&pDev->SPIDrv, TransmitData, 2);   
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  
  
  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }  
  
  /*Верификация данных*/
  Res = prvDeviceCC1200ReadRegister(pDev, AddrReg, &VerifyData, NULL, pExCode);

  if(Res != FUNC_OK)
    return Res;
  
  if(VerifyData != ValueReg)  
  {
    *pExCode = DEVICE_CC1200_ERROR_WRITE_SPI;
    return FUNC_ERROR;        
  }
  
  return FUNC_OK;
}

/*Установка режима приёмопередатчика*/
static uint32_t prvDeviceCC1200SetMode(DeviceCC1200* pDev, DeviceCC1200Mode DevMode, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  
  /*Установка режима приёмопередатчика*/
  uint32_t Res;
  
  if(DevMode == DEVICE_CC1200_TRANSMIT)
    Res = prvDeviceCC1200CommandStrobes(pDev, STX, pExCode);
  else if(DevMode == DEVICE_CC1200_RECEIVE)
    Res = prvDeviceCC1200CommandStrobes(pDev, SRX, pExCode);
  else if(DevMode == DEVICE_CC1200_IDLE)
    Res = prvDeviceCC1200CommandStrobes(pDev, SIDLE, pExCode);
  
  if(Res != FUNC_OK)  
    return Res;
  
  if(pDev->DevMode != DevMode)
    osDelay(1);
  
  pDev->DevMode = DevMode;
  
  return FUNC_OK;
}

/*Функция ожидания готовности приёмопередатчика*/
static uint32_t prvDeviceCC1200WaitStausByte(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode, uint32_t Timeout)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE;   
  
  uint32_t Res;
  uint32_t WaitTick = Timeout;
  uint8_t ChipStatusByte;
  
  /*Ожидание готовности приёмопередатчика*/
  while(TRUE)
  {
    Res = DeviceCC1200ReadChipStatus(pDev, &ChipStatusByte, pExCode);
    
    if(Res != FUNC_OK)
      return Res;
    
    if(__CHIP_RDYn(ChipStatusByte) == CHIP_READY)
      break;
    
    if(Timeout != CC1200_TIMEOUT_INFINITE)
    {
      if(!(--WaitTick))
      {
        *pExCode = DEVICE_CC1200_ERROR_TIMEOUT;
        return FUNC_ERROR;
      }      
    }       
  }
  
  return FUNC_OK;
}

/*Очистка буфера RX*/
static uint32_t prvDeviceCC1200FlushRXFIFO(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode)
{
  /*Проверка входных параметров*/
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;
  
  *pExCode  = DEVICE_CC1200_NOT_CODE; 
  uint32_t Res;  

  /*Очистка буфера RX*/   
  Res = prvDeviceCC1200CommandStrobes(pDev, SFRX, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  return FUNC_OK;     
}
           
/** @name Значения конфигурационных регистров по умолчанию
*/                        
///@{
#define DEF_SYNC_CFG1                   0xA8
#define DEF_SYNC_CFG0                   0x13
#define DEF_DEVIATION_M                 0x99
#define DEF_MODCFG_DEV_E                0x05
#define DEF_DCFILT_CFG                  0x26
#define DEF_PREAMBLE_CFG0               0x8A
#define DEF_IQIC                        0x00
#define DEF_CHAN_BW                     0x02
#define DEF_MDMCFG2                     0x00
#define DEF_MDMCFG1                     0x42
#define DEF_MDMCFG0                     0x05
#define DEF_FREQ2                       0x56
#define DEF_FREQ1                       0x99
#define DEF_FREQ0                       0x98
#define DEF_SYMBOL_RATE2                0x80
#define DEF_SYMBOL_RATE1                0x62
#define DEF_SYMBOL_RATE0                0x4E
#define DEF_AGC_REF                     0x2F
#define DEF_AGC_CS_THR                  0xEC
#define DEF_AGC_CFG1                    0x16
#define DEF_AGC_CFG0                    0x84
#define DEF_FIFO_CFG                    0x80
#define DEF_FS_CFG                      0x14
#define DEF_PKT_CFG2                    0x00
#define DEF_PKT_CFG1                    0x0A
#define DEF_PKT_CFG0                    0x20
#define DEF_PA_CFG0                     0x54
#define DEF_PKT_LEN                     0xFF
#define DEF_IF_MIX_CFG                  0x18
#define DEF_TOC_CFG                     0x03
#define DEF_IF_ADC1                     0xEE
#define DEF_IF_ADC0                     0x10
#define DEF_FS_CAL1                     0x40
#define DEF_FS_CAL0                     0x0E
#define DEF_FS_DIVTWO                   0x03
#define DEF_FS_DSM0                     0x33
#define DEF_FS_PFD                      0x00
#define DEF_FS_PRE                      0x6E
#define DEF_FS_PRE                      0x6E
#define DEF_FS_REG_DIV_CML              0x1C
#define DEF_FS_SPARE                    0xAC
#define DEF_FS_VCO0                     0xB5
#define DEF_IFAMP                       0x0D
#define DEF_XOSC5                       0x0E
#define DEF_XOSC1                       0x03
///@}

/*Установка параметров приёмопередатчика по умолчанию*/
static uint32_t prvDeviceCC1200SetDefsParams(DeviceCC1200* pDev, ExtCodeDeviceCC1200* pExCode)
{  
  /*Проверка входных параметров*/  
  if(!pDev || !pExCode)
    return FUNC_INVALID_PARAM;  

  /*Конфигурация регистров из стандартной области памяти*/
  uint32_t Res;

  Res = prvDeviceCC1200WriteRegister(pDev, DEV_ADDR, pDev->DevAddr, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, SYNC_CFG1, DEF_SYNC_CFG1, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, SYNC_CFG0, DEF_SYNC_CFG0, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, DEVIATION_M, DEF_DEVIATION_M, pExCode);
  
  if(Res != FUNC_OK)
    return Res;

  Res = prvDeviceCC1200WriteRegister(pDev, MODCFG_DEV_E, DEF_MODCFG_DEV_E, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, DCFILT_CFG, DEF_DCFILT_CFG, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, PREAMBLE_CFG0, DEF_PREAMBLE_CFG0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, IQIC, DEF_IQIC, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, CHAN_BW, DEF_CHAN_BW, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, MDMCFG1, DEF_MDMCFG1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  Res = prvDeviceCC1200WriteRegister(pDev, MDMCFG0, DEF_MDMCFG0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, MDMCFG2, DEF_MDMCFG2, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FREQ2, DEF_FREQ2, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FREQ1, DEF_FREQ1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FREQ0, DEF_FREQ0, pExCode);

  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, SYMBOL_RATE2, DEF_SYMBOL_RATE2, pExCode);

  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, SYMBOL_RATE1, DEF_SYMBOL_RATE1, pExCode);

  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, SYMBOL_RATE0, DEF_SYMBOL_RATE0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, AGC_REF, DEF_AGC_REF, pExCode);

  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, AGC_CS_THR, DEF_AGC_CS_THR, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, AGC_CFG1, DEF_AGC_CFG1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, AGC_CFG0, DEF_AGC_CFG0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, FIFO_CFG, DEF_FIFO_CFG, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, FS_CFG, DEF_FS_CFG, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, PKT_CFG2, DEF_PKT_CFG2, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, PKT_CFG1, DEF_PKT_CFG1, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, PKT_CFG0, DEF_PKT_CFG0, pExCode);   
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, PA_CFG0, DEF_PA_CFG0, pExCode); 
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteRegister(pDev, PKT_LEN, DEF_PKT_LEN, pExCode);  
  
  if(Res != FUNC_OK)
    return Res;  
  
  /*Конфигурация регистров из расширенной области памяти*/
    
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, IF_MIX_CFG, DEF_IF_MIX_CFG, pExCode); 
  
  if(Res != FUNC_OK)
    return Res;  
    
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, TOC_CFG, DEF_TOC_CFG, pExCode); 
  
  if(Res != FUNC_OK)
    return Res;  
    
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, IF_ADC1, DEF_IF_ADC1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
    
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, IF_ADC0, DEF_IF_ADC0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  

  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_CAL1, DEF_FS_CAL1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  

  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_CAL0, DEF_FS_CAL0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DIVTWO, DEF_FS_DIVTWO, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  

  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_DSM0, DEF_FS_DSM0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  

  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_PFD, DEF_FS_PFD, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
 
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_PRE, DEF_FS_PRE, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_REG_DIV_CML, DEF_FS_REG_DIV_CML, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  

  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_SPARE, DEF_FS_SPARE, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  

  Res = prvDeviceCC1200WriteExtendedRegister(pDev, FS_VCO0, DEF_FS_VCO0, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
 
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, IFAMP, DEF_IFAMP, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, XOSC5, DEF_XOSC5, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
  
  Res = prvDeviceCC1200WriteExtendedRegister(pDev, XOSC1, DEF_XOSC1, pExCode);
  
  if(Res != FUNC_OK)
    return Res;  
    
  return FUNC_OK;
}

/*Установка команды для приёмопередатчика*/
static uint32_t prvDeviceCC1200CommandStrobes(DeviceCC1200* pDev, uint8_t AddressCmd, ExtCodeDeviceCC1200* pExCode)
{
  /*Check input parameters*/  
  if(!pDev)
    return FUNC_INVALID_PARAM;
  
  uint8_t TransmitData = AddressCmd | WRITE_SINGLE_ACCESS;
  
  HAL_StatusTypeDef Res;
  osStatus osRes;
  Res = HAL_SPI_Transmit_DMA(&pDev->SPIDrv, &TransmitData, 1);    
  
  if(Res != HAL_OK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;      
  }  
  
  osRes = (osStatus)osSemaphoreWait(pDev->SemIDSyncDMA, TIME_WAIT_SEM_SPI_DMA);
  
  if(osRes != osOK)
  {
    *pExCode = DEVICE_CC1200_ERROR_PROCESS_DMA;
    return FUNC_ERROR;    
  }
    
  return FUNC_OK;
}

/**Обработчик прерывания SPI на окончание передачи
  \param[in] hspi указатель на структуру драйвера SPI
*/ 
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  DeviceCC1200* pDev = (DeviceCC1200*)hspi->pUserData;
  osStatus osRes;
  
  osRes = osSemaphoreRelease(pDev->SemIDSyncDMA);
  /*
  if(osRes != osOK)
  {
    char LogBuffer[LOG_BUFFER_MAX_SIZE];
    snprintf(LogBuffer,LOG_BUFFER_MAX_SIZE,"HAL_SPI_TxCpltCallback::Error osSemaphoreRelease! osRes = %x\r\n",(uint32_t)osRes);
    vTCPLogPrintf(LogBuffer);
  }
  */  
  return;  
}

/**Обработчик прерывания SPI на окончание приёмопередачи
  \param[in] hspi указатель на структуру драйвера SPI
*/ 
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  DeviceCC1200* pDev = (DeviceCC1200*)hspi->pUserData;
  osStatus osRes;
  
  osRes = osSemaphoreRelease(pDev->SemIDSyncDMA);
  /*
  if(osRes != osOK)
  {
    char LogBuffer[LOG_BUFFER_MAX_SIZE];
    snprintf(LogBuffer,LOG_BUFFER_MAX_SIZE,"HAL_SPI_TxRxCpltCallback::Error osSemaphoreRelease! osRes = %x\r\n",(uint32_t)osRes);
    vTCPLogPrintf(LogBuffer);
  }
  */  
  return;
}

/**
@}
*/