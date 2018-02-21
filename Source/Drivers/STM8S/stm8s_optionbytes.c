/**
  \file    stm8s_optionbytes.c 
  \brief   Исполняемый файл драйвера управления Option Bytes ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "stm8s_optionbytes.h"
#include "BitsMaskOperations.h"

/** @name Регистры Option Bytes
*/                        
///@{
#define OPT0     (((volatile char*)(0x004800)))
#define OPT1     (((volatile char*)(0x004801)))
#define NOPT1    (((volatile char*)(0x004802)))
#define OPT2     (((volatile char*)(0x004803)))
#define NOPT2    (((volatile char*)(0x004804)))
#define OPT3     (((volatile char*)(0x004805)))
#define NOPT3    (((volatile char*)(0x004806)))
#define OPT4     (((volatile char*)(0x004807)))
#define NOPT4    (((volatile char*)(0x004808)))
#define OPT5     (((volatile char*)(0x004809)))
#define NOPT5    (((volatile char*)(0x00480A)))
#define OPT6     (((volatile char*)(0x00480B)))
#define NOPT6    (((volatile char*)(0x00480C)))
#define OPT7     (((volatile char*)(0x00480D)))
#define NOPT7    (((volatile char*)(0x00480E)))
#define OPTBL    (((volatile char*)(0x00487E)))
#define NOPTBL   (((volatile char*)(0x00487F)))
///@}

/*Настройка доступа к флеш-памяти*/
uint32_t stm8s_optionbytes_SetFlashWait(char WaitState)
{
  CHANGE_REG(OPT7,WaitState);
  CHANGE_REG(NOPT7,(char)(~WaitState));
  
  if(GET_REG(OPT7) != WaitState || GET_REG(NOPT7) != (char)(~WaitState))
    return FUNC_ERROR;
  
  return FUNC_OK;
}