/**
  \file    stm8s_gpio.h
  \brief   Заголовочный файл драйвера GPIO ядра STM8S
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __STM8S_GPIO__
#define __STM8S_GPIO__

#include "TypesDefine.h"

/**
  \addtogroup module_STM8S_GPIO
@{
*/

///Режим работы GPIO
typedef enum _GPIOMode {
  GPIO_FLOATING_INPUT                = 0x0,             ///<Плавающий вход
  GPIO_INPUT_WITH_PULLUP             = 0x2,             ///<Вход с подтяжкой к питанию
  GPIO_OUTPUT_PSEUDO_OPEN_DRAIN      = 0x1,             ///<Выход с эмуляцией режима "с открытым стоком"
  GPIO_OUTPUT_PUSHPULL               = 0x3              ///<Двухтактный выход
} GPIOMode;

///Скорость работы GPIO
typedef enum _GPIOSpeed {
  GPIO_SPEED_2MHZ                    = 0x0,             ///<2 МГц
  GPIO_SPEED_10MHZ                   = 0x1              ///<10 МГц
} GPIOSpeed;

///Наименование порта GPIO
typedef enum _GPIOPort {
  GPIO_PORT_A                        = 0x00,            ///<Порт A
  GPIO_PORT_B                        = 0x05,            ///<Порт B  
  GPIO_PORT_C                        = 0x0A,            ///<Порт C
  GPIO_PORT_D                        = 0x0F,            ///<Порт D
  GPIO_PORT_E                        = 0x14,            ///<Порт E
  GPIO_PORT_F                        = 0x19,            ///<Порт F
  GPIO_PORT_G                        = 0x1E,            ///<Порт G
  GPIO_PORT_H                        = 0x23,            ///<Порт H
  GPIO_PORT_I                        = 0x28             ///<Порт I
} GPIOPort;

///Структура GPIO
typedef struct _GPIOType {
  GPIOMode          Mode;                               ///<Режим 
  GPIOSpeed         Speed;                              ///<Скорость
  HwrIntFunc        FuncCallback;                       ///<Обработчик прерываний
  void*             UserData;                           ///<Указатель на пользовательские данные          
  char              NumPin;                             ///<Номер вывода
  GPIOPort          NumPort;                            ///<Номер порта
} GPIOType;

#define GPIO_OUTPUT_LOW             0                   ///<Выход "0"
#define GPIO_OUTPUT_HIGH            1                   ///<Выход "1"

/**Инициализация GPIO
  \param[in] GPIODef структура GPIO
  \return Результат выполнения функции
*/
uint32_t stm8s_gpio_Init(GPIOType* GPIODef);

/**Установка выхода GPIO
  \param[in] NumPort номер порта GPIO
  \param[in] NumPin номер вывода GPIO
  \param[in] GPIOValue значение выхода
  \return Результат выполнения функции
*/
uint32_t stm8s_gpio_Set(GPIOPort NumPort, char NumPin, char GPIOValue);

/**
@}
*/

#endif