/**
  \file    GPIOfun.h 
  \brief   Заголовочный файл функций для работы с GPIO
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef _GPIOFUN_
#define _GPIOFUN_

/**Получение номера GPIO по строковому описанию
  \param[in] StrGPIOPin строковое описание номера GPIO
  \param[out] ppGPIOPort указатель на порт GPIO
  \param[out] pGPIONum номер GPIO
  \return Результат выполнения функции 
*/
uint32_t GPIOGetPort(const char* StrGPIOPin, GPIO_TypeDef** ppGPIOPort, uint16_t* pGPIONum);

#endif
