/**
  \file    stm8s_itc.c 
  \brief   Исполняемый файл контроллера прерываний ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/
#include "TypesDefine.h"

/*Включить обработку прерываний*/
void stm8s_itc_InterruptsEnable(void)
{
  _asm("RIM");  
}