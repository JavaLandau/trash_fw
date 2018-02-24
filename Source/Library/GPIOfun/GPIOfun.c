/**
  \file    GPIOfun.c 
  \brief   Исполняемый файл функций для работы с GPIO
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "stm32f0xx_hal.h"
#include "TypesDefine.h"
#include "GPIOfun.h"

/** @name Символьное представление портов GPIO
*/                        
///@{
#define PORT_A                          'A'
#define PORT_B                          'B'
#define PORT_C                          'C'
#define PORT_D                          'D'
#define PORT_E                          'E'
#define PORT_F                          'F'
#define PORT_G                          'G'
///@}

///Номер GPIO в порте
#define GET_GPIO_NUM(NUM)               (GPIO_PIN_0 << (NUM))

/*Получение номера GPIO по строковому описанию*/
uint32_t GPIOGetPort(const char* StrGPIOPin, GPIO_TypeDef** ppGPIOPort, uint16_t* pGPIONum)
{
  /*Проверка входных параметров*/
  if(!ppGPIOPort || ! pGPIONum || !StrGPIOPin)
    return FUNC_INVALID_PARAM;
  
  if(*ppGPIOPort)
    return FUNC_INVALID_PARAM;
  
  char GPIOPort;
  uint32_t GPIONum;
  uint32_t Res;
  
  Res = sscanf(StrGPIOPin,"P%c%d",&GPIOPort,&GPIONum);
  
  if(Res != 2)
    return FUNC_ERROR;
  
  switch(GPIOPort)
  {
#ifdef GPIOA  
  case PORT_A:
    *ppGPIOPort = GPIOA;
    break;
#endif  
#ifdef GPIOB      
  case PORT_B:
    *ppGPIOPort = GPIOB;
    break;
#endif
#ifdef GPIOC      
  case PORT_C:
    *ppGPIOPort = GPIOC;
    break;
#endif
#ifdef GPIOD      
  case PORT_D:
    *ppGPIOPort = GPIOD;
    break;
#endif
#ifdef GPIOE      
  case PORT_E:
    *ppGPIOPort = GPIOE;
    break;
#endif
#ifdef GPIOF      
  case PORT_F:
    *ppGPIOPort = GPIOF;
    break;
#endif
#ifdef GPIOG      
  case PORT_G:
    *ppGPIOPort = GPIOG;
    break;
#endif
  default:
    return FUNC_INVALID_PARAM;
  }
  
  *pGPIONum = GET_GPIO_NUM(GPIONum);
  
  return FUNC_OK;  
}