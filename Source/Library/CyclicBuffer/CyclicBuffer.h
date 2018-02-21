/**
  \file    CyclicBuffer.h 
  \brief   Заголовочный файл циклического буфера
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __CYCLIC_BUFFER__
#define __CYCLIC_BUFFER__

#include "stm32f4xx_hal.h"

///Структура циклического буфера
typedef struct
{
  uint32_t      IndexWrite;             ///<Номер байта в буфере, который будет записан 
  uint32_t      IndexRead;              ///<Номер байта в буфере, который будет прочитан
  uint8_t*      pBuffer;                ///<Указатель на динамический массив байт
  uint32_t      SizeBuffer;             ///<Размер буфера
  uint8_t       isEmpty;                ///<Флаг того, что буфер пустой
  uint8_t       isFull;                 ///<Флаг того, что буфер полный
} CyclicBuff;

///Специальные коды ошибок
typedef enum {
  CYCLIC_BUFF_NOT_CODE = 0,             ///<Ошибок нет
  CYCLIC_BUFF_UNDERFLOW,                ///<Чтение данных из пустого буфера
  CYCLIC_BUFF_OVERFLOW,                 ///<Переполнение буфера
  CYCLIC_BUFF_DONT_INIT                 ///<Буфер не проинициализирован
} ExtCodeCyclicBuff;

/**Инициализация циклического буфера
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \param[in] SizeBuff размер циклического буфера в байтах
  \return Результат выполнения функции 
*/
uint8_t CyclicBufferCreate(CyclicBuff* pCyclicBuff, uint32_t SizeBuff);

/**Запись байта в циклический буфер
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \param[in] PutByte записываемый байт
  \param[out] pExtCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint8_t CyclicBufferPut(CyclicBuff* pCyclicBuff, uint8_t PutByte, ExtCodeCyclicBuff* pExtCode);


/**Чтение байта из циклического буфера
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \param[in] pGetByte считываемый байт
  \param[out] pExtCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint8_t CyclicBufferGet(CyclicBuff* pCyclicBuff, uint8_t* pGetByte, ExtCodeCyclicBuff* pExtCode);

/**Сброс циклического буфера
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \param[out] pExtCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint8_t CyclicBufferReset(CyclicBuff* pCyclicBuff, ExtCodeCyclicBuff* pExtCode);

/**Размер циклического буфера, доступный для записи
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \param[out] pWriteSize размер буфера, доступный для записи
  \param[out] pExtCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint8_t CyclicBufferGetWriteSize(CyclicBuff* pCyclicBuff, uint32_t* pWriteSize, ExtCodeCyclicBuff* pExtCode);

/**Размер циклического буфера, доступный для чтения
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \param[out] pReadSize размер буфера, доступный для чтения
  \param[out] pExtCode специальный код ошибки выполнения функции
  \return Результат выполнения функции
*/
uint8_t CyclicBufferGetReadSize(CyclicBuff* pCyclicBuff, uint32_t* pReadSize, ExtCodeCyclicBuff* pExtCode);

/**Деинициализация циклического буфера
  \param[in] pCyclicBuff указатель на структуру циклического буфера
  \return Результат выполнения функции
*/
uint8_t CyclicBufferDestroy(CyclicBuff* pCyclicBuff);

#endif