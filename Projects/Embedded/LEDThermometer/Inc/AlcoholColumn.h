/**
  \file    AlcoholColumn.h 
  \brief   Заголовочный файл модуля управления светодиодным спиртовым столбом
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef _ALCOHOL_COLUMN_
#define _ALCOHOL_COLUMN_

/**
  \addtogroup module_AlcoholColumn
@{
*/

/** @name Константы, задающие температурный диапазон
*/                        
///@{
#define TEMPERATURE_MAX         40
#define TEMPERATURE_MIN         -5
///@}

/**Инициализация модуля управления светодиодным спиртовым столбом
  \return Результат выполнения функции 
*/
uint32_t AlcoholColumnCreate(void);

/**Установка заданного значения температуры
  \param[in] Value значение температуры
  \return Результат выполнения функции 
*/
uint32_t AlcoholColumnSet(int8_t Value);

/**Сброс значения температуры
  \return Результат выполнения функции 
*/
uint32_t AlcoholColumnReset();

/**  
@}
*/

#endif