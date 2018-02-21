/**
  \file    CyclicBuffer.c 
  \brief   Исполняемый файл циклического буфера
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include <stdlib.h>

#include "stm32f4xx_hal.h"
#include "CyclicBuffer.h"
#include "TypesDefine.h"

/*Инициализация циклического буфера*/
uint8_t CyclicBufferCreate(CyclicBuff* pCyclicBuff, uint32_t SizeBuff)
{
  if(!pCyclicBuff || !SizeBuff)
    return FUNC_INVALID_PARAM;
  
  pCyclicBuff->pBuffer = (uint8_t*)malloc(SizeBuff*sizeof(uint8_t));
  
  if(!pCyclicBuff->pBuffer)
    return FUNC_HEAP_MEM_ERROR;
  
  pCyclicBuff->IndexRead = 0;
  pCyclicBuff->IndexWrite = 0;
  pCyclicBuff->isEmpty = TRUE;
  pCyclicBuff->isFull = FALSE;
  pCyclicBuff->SizeBuffer = SizeBuff;
  
  return FUNC_OK;  
}

/*Запись байта в циклический буфер*/
uint8_t CyclicBufferPut(CyclicBuff* pCyclicBuff, uint8_t PutByte, ExtCodeCyclicBuff* pExtCode)
{
  if(!pCyclicBuff || !pExtCode)
    return FUNC_INVALID_PARAM;  
  
  *pExtCode = CYCLIC_BUFF_NOT_CODE;
  
  if(!pCyclicBuff->pBuffer)
  {
    *pExtCode = CYCLIC_BUFF_DONT_INIT;
    return FUNC_ERROR;
  }
  
  if(pCyclicBuff->IndexWrite == pCyclicBuff->IndexRead && pCyclicBuff->isFull)
  {
    *pExtCode = CYCLIC_BUFF_OVERFLOW;
    return FUNC_ERROR;    
  }

  pCyclicBuff->pBuffer[pCyclicBuff->IndexWrite++] = PutByte;
  pCyclicBuff->isEmpty = FALSE;  
 
  if(pCyclicBuff->IndexWrite == pCyclicBuff->SizeBuffer)
    pCyclicBuff->IndexWrite = 0;    
    
  if(pCyclicBuff->IndexWrite == pCyclicBuff->IndexRead)
    pCyclicBuff->isFull = TRUE;  
  
  return FUNC_OK;
}

/*Чтение байта из циклического буфера*/
uint8_t CyclicBufferGet(CyclicBuff* pCyclicBuff, uint8_t* pGetByte, ExtCodeCyclicBuff* pExtCode)
{
  if(!pCyclicBuff || !pExtCode || !pGetByte)
    return FUNC_INVALID_PARAM;  
  
  *pExtCode = CYCLIC_BUFF_NOT_CODE;
  
  if(!pCyclicBuff->pBuffer)
  {
    *pExtCode = CYCLIC_BUFF_DONT_INIT;
    return FUNC_ERROR;
  }
  
  if(pCyclicBuff->IndexWrite == pCyclicBuff->IndexRead && pCyclicBuff->isEmpty)
  {
    *pExtCode = CYCLIC_BUFF_UNDERFLOW;    
    return FUNC_ERROR;    
  }
  
  *pGetByte = pCyclicBuff->pBuffer[pCyclicBuff->IndexRead++];
  pCyclicBuff->isFull = FALSE;
 
  if(pCyclicBuff->IndexRead == pCyclicBuff->SizeBuffer)
    pCyclicBuff->IndexRead = 0;      
  
  if(pCyclicBuff->IndexWrite == pCyclicBuff->IndexRead)
    pCyclicBuff->isEmpty = TRUE;
   
  return FUNC_OK;
}

/*Сброс циклического буфера*/
uint8_t CyclicBufferReset(CyclicBuff* pCyclicBuff, ExtCodeCyclicBuff* pExtCode)
{
  if(!pCyclicBuff || !pExtCode)
    return FUNC_INVALID_PARAM;  
  
  *pExtCode = CYCLIC_BUFF_NOT_CODE;
  
  if(!pCyclicBuff->pBuffer)
  {
    *pExtCode = CYCLIC_BUFF_DONT_INIT;
    return FUNC_ERROR;
  }

  pCyclicBuff->IndexRead = 0;
  pCyclicBuff->IndexWrite = 0;
  pCyclicBuff->isEmpty = TRUE;
  pCyclicBuff->isFull = FALSE;  

  return FUNC_OK;  
}

/*Размер циклического буфера, доступный для записи*/
uint8_t CyclicBufferGetWriteSize(CyclicBuff* pCyclicBuff, uint32_t* pWriteSize, ExtCodeCyclicBuff* pExtCode)
{
  if(!pCyclicBuff || !pExtCode || !pWriteSize)
    return FUNC_INVALID_PARAM;  
  
  *pExtCode = CYCLIC_BUFF_NOT_CODE;
  
  if(!pCyclicBuff->pBuffer)
  {
    *pExtCode = CYCLIC_BUFF_DONT_INIT;
    return FUNC_ERROR;
  } 
  
  if(pCyclicBuff->IndexRead > pCyclicBuff->IndexWrite)
    *pWriteSize = pCyclicBuff->IndexRead - pCyclicBuff->IndexWrite;
  else if(pCyclicBuff->IndexRead < pCyclicBuff->IndexWrite)
    *pWriteSize = pCyclicBuff->SizeBuffer - pCyclicBuff->IndexWrite + pCyclicBuff->IndexRead;
  else
  {
    if(pCyclicBuff->isEmpty)
      *pWriteSize = pCyclicBuff->SizeBuffer;
    else
      *pWriteSize = 0;          
  }
    
  return FUNC_OK;
}

/*Размер циклического буфера, доступный для чтения*/
uint8_t CyclicBufferGetReadSize(CyclicBuff* pCyclicBuff, uint32_t* pReadSize, ExtCodeCyclicBuff* pExtCode)
{
  if(!pCyclicBuff || !pExtCode || !pReadSize)
    return FUNC_INVALID_PARAM;  
  
  *pExtCode = CYCLIC_BUFF_NOT_CODE;
  
  if(!pCyclicBuff->pBuffer)
  {
    *pExtCode = CYCLIC_BUFF_DONT_INIT;
    return FUNC_ERROR;
  } 
  
  if(pCyclicBuff->IndexWrite > pCyclicBuff->IndexRead)
    *pReadSize = pCyclicBuff->IndexWrite - pCyclicBuff->IndexRead;
  else if(pCyclicBuff->IndexWrite < pCyclicBuff->IndexRead)
    *pReadSize = pCyclicBuff->SizeBuffer - pCyclicBuff->IndexRead + pCyclicBuff->IndexWrite;
  else
  {
    if(pCyclicBuff->isFull)
      *pReadSize = pCyclicBuff->SizeBuffer;
    else
      *pReadSize = 0;      
  }
    
  return FUNC_OK;
}

/*Деинициализация циклического буфера*/
uint8_t CyclicBufferDestroy(CyclicBuff* pCyclicBuff)
{
  if(!pCyclicBuff)
    return FUNC_INVALID_PARAM;    
  
  free(pCyclicBuff->pBuffer);
  pCyclicBuff->pBuffer = 0;
  
  return FUNC_OK;  
}