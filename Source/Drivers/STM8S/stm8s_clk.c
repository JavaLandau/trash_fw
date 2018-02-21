/**
  \file    stm8s_clk.c 
  \brief   Исполняемый файл драйвера системы тактирования ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "stm8s_optionbytes.h"
#include "stm8s_flash.h"
#include "stm8s_clk.h"
#include "BitsMaskOperations.h"

/** @name Регистры CLK
*/                        
///@{
#define CLK_ICKR     (((volatile char*)(0x0050C0)))
#define CLK_ECKR     (((volatile char*)(0x0050C1)))
#define CLK_CMSR     (((volatile char*)(0x0050C3)))
#define CLK_SWR      (((volatile char*)(0x0050C4)))
#define CLK_SWCR     (((volatile char*)(0x0050C5)))
#define CLK_CKDIVR   (((volatile char*)(0x0050C6)))
#define CLK_PCKENR1  (((volatile char*)(0x0050C7)))
#define CLK_CSSR     (((volatile char*)(0x0050C8)))
#define CLK_CCOR     (((volatile char*)(0x0050C9)))
#define CLK_PCKENR2  (((volatile char*)(0x0050CA)))
#define CLK_CANCCR   (((volatile char*)(0x0050CB)))
#define CLK_HSITRIMR (((volatile char*)(0x0050CC)))
#define CLK_SWIMCCR  (((volatile char*)(0x0050CD)))
///@}

/** @name Значения битов регистров CLK
*/                        
///@{
#define HSIEN         0
#define HSIRDY        1
#define FHWU          2
#define LSIEN         3
#define LSIRDY        4
#define REGAH         5

#define HSEEN         0
#define HSERDY        1

#define SWBSY         0
#define SWEN          1
#define SWIEN         2
#define SWIF          3

#define HSI_SELECT    0xE1
#define LSI_SELECT    0xD2
#define HSE_SELECT    0xB4
///@}

/*Назначение HSE источником тактирования ядра*/
uint32_t stm8s_clk_SetHSEMasterClock(void)
{
  uint32_t Res;
  
  /*Установка HSE источником тактирования*/
  CHANGE_REG(CLK_SWR,HSE_SELECT);

  if(GET_REG(CLK_SWR) != HSE_SELECT) 
    return FUNC_ERROR;
    
  /*Ожидание стабилизации генератора*/  
  while(!GET_REG_BIT(CLK_SWCR,SWIF))
    _asm("NOP");	
  
  RESET_REG_BIT(CLK_SWCR,SWIF);

  if(GET_REG_BIT(CLK_SWCR,SWIF) != RESET) 
    return FUNC_ERROR;

  /*Открыть доступ к записи option bytes*/
  Res = stm8s_flash_OptionBytesEnable();
  
  if(Res != FUNC_OK)
    return Res;
  
  /*Изменение настроек доступа к флеш-памяти в случае Fclk > 16 MHz*/
  Res = stm8s_optionbytes_SetFlashWait(OPT7_WAIT_1);
	
  if(Res != FUNC_OK)
    return Res;          
  
  /*Закрыть доступ к записи option bytes*/
  Res = stm8s_flash_OptionBytesDisable();
  
  if(Res != FUNC_OK)
    return Res;          
         
  /*Переключить систему тактирования STM8S на HSE*/
  SET_REG_BIT(CLK_SWCR,SWEN);	
			
  return FUNC_OK;
}