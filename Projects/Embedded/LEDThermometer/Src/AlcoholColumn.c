/**
  \file    AlcoholColumn.с
  \brief   Исполняемый файл модуля управления светодиодным спиртовым столбом
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#include "DeviceMBI5039.h"

/**
  \defgroup module_service_AlcoholColumn Служебные функции для работы с модулем управления светодиодным спиртовым столбом
  \brief Модуль служебных функций, необходимых для работы с модулем управления светодиодным спиртовым столбом
@{
*/

///Количество последовательно подключенных микросхем
#define NUM_DEV_MBI5039         3        

///Объект драйвера светодиодов MBI5039
static DeviceMBI5039 DevMBI5039;

/**Вычисление кода для драйвера светодиодов по приведенному значению температуры
  \param[in] NumDev номер микросхемы MBI5039
  \param[in] Value приведенное значение температуры
  \return Результат выполнения функции 
*/
static uint16_t prvGetDevOutput(uint8_t NumDev, uint8_t Value)
{
  if(NumDev != 3)
  {
    if(Value != 7 && Value != 9)
      return ((1 << Value) - 1);
    else
      return ((1 << (Value - 1)) - 1)|(1 << Value);      
  }else
  {
    if(Value != 6 && Value != 8)
      return (((1 << Value) - 1) << 1);
    else
      return (((1 << (Value-1)) - 1) << 1)|(1 << (Value + 1));
  }
}

/**
@}
  \defgroup module_AlcoholColumn Интерфейсные функции для работы с модулем управления светодиодным спиртовым столбом
  \brief Модуль, предоставляющий пользователю необходимый функционал для работы с с модулем управления светодиодным спиртовым столбом
@{
*/

/*Инициализация модуля управления светодиодным спиртовым столбом*/
uint32_t AlcoholColumnCreate(void)
{
  uint32_t Res;
  DeviceMBI5039Param DevParam;  
  ExtCodeDeviceMBI5039 MBI5039ExCode;
  DevParam.pInstanceSPIDrv = SPI1;
  DevParam.BaudRateSPIPrescaler = 1000000;
  DevParam.devNum = NUM_DEV_MBI5039;
  DevParam.GPIONSSPin = "PA4";
  
  Res = DeviceMBI5039Create(&DevParam, &DevMBI5039, &MBI5039ExCode);
  
  if(Res != FUNC_OK)
    return Res;
  
  
  Res = DeviceMBI5039Reset(&DevMBI5039, &MBI5039ExCode);
  
  return Res;
}

/*Сброс значения температуры*/
uint32_t AlcoholColumnReset()
{
  uint32_t Res;
  ExtCodeDeviceMBI5039 MBI5039ExCode;
  Res = DeviceMBI5039Reset(&DevMBI5039, &MBI5039ExCode);
  return Res;  
}

/*Установка заданного значения температуры*/
uint32_t AlcoholColumnSet(int8_t Value)
{
  uint8_t IntValue = Value + 6;
  uint16_t DevOutput[NUM_DEV_MBI5039] = {0, 0, 0};
  ExtCodeDeviceMBI5039 MBI5039ExCode;
  uint32_t Res;
  
  if(IntValue > 16)  
  {
    DevOutput[2] = 0xFFFF;
    
    if(IntValue > 32)
    {
      DevOutput[1] = 0xFFFF;
      DevOutput[0] = prvGetDevOutput(3, IntValue - 32);
    } else
    {
      DevOutput[1] = prvGetDevOutput(2, IntValue - 16);
    }
  }else
  {
    DevOutput[2] = prvGetDevOutput(1, IntValue);
  }
    
  Res = DeviceMBI5039Set(&DevMBI5039, DevOutput, &MBI5039ExCode);
  return Res;
}

/**
@}
*/