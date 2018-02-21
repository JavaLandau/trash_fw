/**
  \file    stm8s_tim.c 
  \brief   Исполняемый файл драйвера таймера ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "BitsMaskOperations.h"
#include "stm8s_tim.h"

/** @name Регистры TIM
*/                        
///@{
//TIM1
#define TIM1_CR1     (((volatile char*)(0x005250)))
#define TIM1_CR2     (((volatile char*)(0x005251)))
#define TIM1_SMCR    (((volatile char*)(0x005252)))
#define TIM1_ETR     (((volatile char*)(0x005253)))
#define TIM1_IER     (((volatile char*)(0x005254)))
#define TIM1_SR1     (((volatile char*)(0x005255)))
#define TIM1_SR2     (((volatile char*)(0x005256)))
#define TIM1_EGR     (((volatile char*)(0x005257)))
#define TIM1_CCMR1   (((volatile char*)(0x005258)))
#define TIM1_CCMR2   (((volatile char*)(0x005259)))
#define TIM1_CCMR3   (((volatile char*)(0x00525A)))
#define TIM1_CCMR4   (((volatile char*)(0x00525B)))
#define TIM1_CCER1   (((volatile char*)(0x00525C)))
#define TIM1_CCER2   (((volatile char*)(0x00525D)))
#define TIM1_CNTRH   (((volatile char*)(0x00525E)))
#define TIM1_CNTRL   (((volatile char*)(0x00525F)))
#define TIM1_PSCRH   (((volatile char*)(0x005260)))
#define TIM1_PSCRL   (((volatile char*)(0x005261)))
#define TIM1_ARRH    (((volatile char*)(0x005262)))
#define TIM1_ARRL    (((volatile char*)(0x005263)))
#define TIM1_RCR     (((volatile char*)(0x005264)))
#define TIM1_CCR1H   (((volatile char*)(0x005265)))
#define TIM1_CCR1L   (((volatile char*)(0x005266)))
#define TIM1_CCR2H   (((volatile char*)(0x005267)))
#define TIM1_CCR2L   (((volatile char*)(0x005268)))
#define TIM1_CCR3H   (((volatile char*)(0x005269)))
#define TIM1_CCR3L   (((volatile char*)(0x00526A)))
#define TIM1_CCR4H   (((volatile char*)(0x00526B)))
#define TIM1_CCR4L   (((volatile char*)(0x00526C)))
#define TIM1_BKR     (((volatile char*)(0x00526D)))
#define TIM1_DTR     (((volatile char*)(0x00526E)))
#define TIM1_OISR    (((volatile char*)(0x00526F)))
//TIM2
#define TIM2_CR1     (((volatile char*)(0x005300)))
#define TIM2_IER     (((volatile char*)(0x005301)))
#define TIM2_SR1     (((volatile char*)(0x005302)))
#define TIM2_SR2     (((volatile char*)(0x005303)))
#define TIM2_EGR     (((volatile char*)(0x005304)))
#define TIM2_CCMR1   (((volatile char*)(0x005305)))
#define TIM2_CCMR2   (((volatile char*)(0x005306)))
#define TIM2_CCMR3   (((volatile char*)(0x005307)))
#define TIM2_CCER1   (((volatile char*)(0x005308)))
#define TIM2_CCER2   (((volatile char*)(0x005309)))
#define TIM2_CNTRH   (((volatile char*)(0x00530A)))
#define TIM2_CNTRL   (((volatile char*)(0x00530B)))
#define TIM2_PSCR    (((volatile char*)(0x00530C)))
#define TIM2_ARRH    (((volatile char*)(0x00530D)))
#define TIM2_ARRL    (((volatile char*)(0x00530E)))
#define TIM2_CCR1H   (((volatile char*)(0x00530F)))
#define TIM2_CCR1L   (((volatile char*)(0x005310)))
#define TIM2_CCR2H   (((volatile char*)(0x005311)))
#define TIM2_CCR2L   (((volatile char*)(0x005312)))
#define TIM2_CCR3H   (((volatile char*)(0x005313)))
#define TIM2_CCR3L   (((volatile char*)(0x005314)))
//TIM3
#define TIM3_CR1     (((volatile char*)(0x005320)))
#define TIM3_IER     (((volatile char*)(0x005321)))
#define TIM3_SR1     (((volatile char*)(0x005322)))
#define TIM3_SR2     (((volatile char*)(0x005323)))
#define TIM3_EGR     (((volatile char*)(0x005324)))
#define TIM3_CCMR1   (((volatile char*)(0x005325)))
#define TIM3_CCMR2   (((volatile char*)(0x005326)))
#define TIM3_CCER1   (((volatile char*)(0x005327)))
#define TIM3_CNTRH   (((volatile char*)(0x005328)))
#define TIM3_CNTRL   (((volatile char*)(0x005329)))
#define TIM3_PSCR    (((volatile char*)(0x00532A)))
#define TIM3_ARRH    (((volatile char*)(0x00532B)))
#define TIM3_ARRL    (((volatile char*)(0x00532C)))
#define TIM3_CCR1H   (((volatile char*)(0x00532D)))
#define TIM3_CCR1L   (((volatile char*)(0x00532E)))
#define TIM3_CCR2H   (((volatile char*)(0x00532F)))
#define TIM3_CCR2L   (((volatile char*)(0x005330)))
//TIM4
#define TIM4_CR1     (((volatile char*)(0x005340)))
#define TIM4_IER     (((volatile char*)(0x005341)))
#define TIM4_SR      (((volatile char*)(0x005342)))
#define TIM4_EGR     (((volatile char*)(0x005343)))
#define TIM4_CNTR    (((volatile char*)(0x005344)))
#define TIM4_PSCR    (((volatile char*)(0x005345)))
#define TIM4_ARR     (((volatile char*)(0x005346)))
///@}

/** @name Значения битов регистров TIM
*/                        
///@{
#define BIF          7
#define TIF          6
#define COMIF        5
#define CC4IF        4
#define CC3IF        3
#define CC2IF        2
#define CC1IF        1
#define UIF          0
///@}

/** @name Указатели на обработчиков прерываний от таймеров 
*/                        
///@{
HwrIntFunc pTIM1IntFunc;
HwrIntFunc pTIM2IntFunc;
HwrIntFunc pTIM3IntFunc;
///@}

/** @name Структуры на каждый аппаратный таймер
*/                        
///@{
const void* TIM1Struct;
const void* TIM2Struct;
const void* TIM3Struct;
///@}

/*Инициализация таймера*/
uint32_t stm8s_tim_Init(TIMType* TIMDef)
{
  /*Проверка входных данных*/
  if(!TIMDef)
    return FUNC_INVALID_PARAM;
  
  if(TIMDef->Instance != TIM1 && TIMDef->Instance != TIM2 &&
     TIMDef->Instance != TIM3 && TIMDef->Instance != TIM4)   
  {
    return FUNC_INVALID_PARAM;
  }
      
  if(TIMDef->Instance == TIM2 || TIMDef->Instance == TIM3)
  {
    if(TIMDef->Prescaler > 0xFF)
      return FUNC_INVALID_PARAM;
    
    if(TIMDef->Instance == TIM2)
    {
      pTIM2IntFunc = TIMDef->FuncCallback;
      TIM2Struct = TIMDef->UserData;
      
      /*Инициализация регистров таймера*/
      CHANGE_REG(TIM2_ARRH, (char)((TIMDef->ARR >> 0x8) & 0xFF));
      CHANGE_REG(TIM2_ARRL, (char)(TIMDef->ARR & 0xFF));
      
      CHANGE_REG(TIM2_PSCR, (char)(TIMDef->Prescaler & 0xFF));
      
      /*Сброс счетчика*/
      RESET_REG(TIM2_CNTRH);
      RESET_REG(TIM2_CNTRL);
                      
      /*Включение обработки прерываний от таймера*/
      CHANGE_REG(TIM2_IER,0x01);      
    } else
    {
      pTIM2IntFunc = TIMDef->FuncCallback;
      TIM2Struct = TIMDef->UserData;
      
      /*Инициализация регистров таймера*/
      CHANGE_REG(TIM3_ARRH, (char)((TIMDef->ARR >> 0x8) & 0xFF));
      CHANGE_REG(TIM3_ARRL, (char)(TIMDef->ARR & 0xFF));
      
      CHANGE_REG(TIM3_PSCR, (char)(TIMDef->Prescaler & 0xFF));
      
      /*Сброс счетчика*/
      RESET_REG(TIM3_CNTRH);
      RESET_REG(TIM3_CNTRL);
                      
      /*Включение обработки прерываний от таймера*/
      CHANGE_REG(TIM3_IER,0x01);            
    }
  } else if(TIMDef->Instance == TIM1)   
  {
    pTIM1IntFunc = TIMDef->FuncCallback;
    TIM1Struct = TIMDef->UserData;
    
    /*Инициализация регистров таймера*/
    CHANGE_REG(TIM1_ARRH, (char)((TIMDef->ARR >> 0x8) & 0xFF));
    CHANGE_REG(TIM1_ARRL, (char)(TIMDef->ARR & 0xFF));
    
    CHANGE_REG(TIM1_PSCRH, (char)((TIMDef->Prescaler >> 0x8) & 0xFF));
    CHANGE_REG(TIM1_PSCRL, (char)(TIMDef->Prescaler & 0xFF));
    
    /*Сброс счетчика*/
    RESET_REG(TIM1_CNTRH);
    RESET_REG(TIM1_CNTRL);

    /*Включение обработки прерываний от таймера*/
    CHANGE_REG(TIM1_IER,0x01);    
  }
  
  return FUNC_OK;
}

/*Запуск счетчика таймера*/
uint32_t stm8s_tim_Start(TIMType* TIMDef)
{
  if(!TIMDef)
    return FUNC_INVALID_PARAM;
  
  if(TIMDef->Instance != TIM1 && TIMDef->Instance != TIM2 &&
     TIMDef->Instance != TIM3 && TIMDef->Instance != TIM4)   
  {
    return FUNC_INVALID_PARAM;
  }

  CHANGE_REG((volatile char*)TIMDef->Instance,0x01);

  return FUNC_OK;
}

/*Остановка счетчика таймера*/
uint32_t stm8s_tim_Stop(TIMType* TIMDef)
{
  if(!TIMDef)
    return FUNC_INVALID_PARAM;
  
  if(TIMDef->Instance != TIM1 && TIMDef->Instance != TIM2 &&
     TIMDef->Instance != TIM3 && TIMDef->Instance != TIM4)   
  {
    return FUNC_INVALID_PARAM;
  }
  
  CHANGE_REG((volatile char*)TIMDef->Instance,0x00);

  return FUNC_OK;
}

/*Обработчик прерываний по событию UPDATE от таймера TIM1*/
void stm8s_TIM1_UpdateInterrupt(void)
{
  RESET_REG_BIT(TIM1_SR1,UIF);
  
  if(pTIM1IntFunc)
    pTIM1IntFunc(TIM1Struct);  
}

/*Обработчик прерываний по событию UPDATE от таймера TIM2*/
void stm8s_TIM2_UpdateInterrupt(void)
{
  RESET_REG_BIT(TIM2_SR1,UIF);
  
  if(pTIM2IntFunc)
    pTIM2IntFunc(TIM2Struct);
}

/*Обработчик прерываний по событию UPDATE от таймера TIM3*/
void stm8s_TIM3_UpdateInterrupt(void)
{
  RESET_REG_BIT(TIM3_SR1,UIF);
  
  if(pTIM1IntFunc)
    pTIM1IntFunc(TIM1Struct);  
}