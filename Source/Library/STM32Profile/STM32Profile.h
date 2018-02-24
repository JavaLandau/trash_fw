/**
  \file STM32Profile.h
  \brief Заголовочный файл профилировщика STM32
  \author  Яковлев В. А.
  \version 1.0
  \date    25.01.2018 
*/

/**
  \addtogroup module_STM32profile
@{
*/

#ifndef __STM32_PROFILE_
#define __STM32_PROFILE_

///Перечень масштабов возвращаемого результата
typedef enum {
  TIME_IN_CYCLES = 0,                           ///<В тактах
  TIME_IN_NSEC,                                 ///<В наносекундах
  TIME_IN_UCSEC,                                ///<В микросекундах
  TIME_IN_MILSEC,                               ///<В миллисекундах
  TIME_IN_SEC                                   ///<В секундах
} STM32TimeScale;

///Структура объекта профиля
typedef struct {
  TIM_HandleTypeDef htim;                       ///<Указатель на объект таймера
  uint32_t TimerFreq;                           ///<Частота тактирования таймера
  uint32_t Period;                              ///<Период србасывания счетчика таймера
  uint32_t TimerCycles;                         ///<Количество тактов
  STM32TimeScale TimeScale;                     ///<Масштаб возвращаемого результата
  uint32_t SelfDelay;                           ///<Задержки, вносимые профилем при вызовах функций, считыванием значений и т. д.
  uint32_t SelfIntDelay;                        ///<Задержки, вносимые профилем при обработке прерывания
} STM32ProfileStruct;

///Перечень ошибок функций профиля
typedef enum {
  STM32_PROFILE_ERROR_NOT_CODE = 0,             ///<Ошибок нет
  STM32_PROFILE_ERROR_TIM1                      ///<Ошибка инициализации таймера TIM1
} STM32ProfileErrorCode;


/** Инициализация профиля
        \param[in] FreqMCU частота микроконтроллера
	\param[in] Prescaler значение предделителя таймера
        \param[in] Period значение периода сбрасывания счетчика таймера
        \param[in] TimeScale масштаб возвращаемого результата
	\param[out] pErrorCode указатель на переменную, содержащую информацию об ошибках выполнения функции	
	\return Признак выполнения функции
        \warning Рассчитываемая частота тактирования таймера микроконтроллера как внутренняя переменная получается,
                 как результат целочисленного деления FreqMCU на Prescaler, поэтому рекомендуется устанавливать значения
                 FreqMCU и Prescaler такими, чтобы они делились нацело
*/
uint32_t STM32ProfileInit(uint32_t FreqMCU, uint32_t Prescaler, uint32_t Period, STM32TimeScale TimeScale, STM32ProfileErrorCode* pErrorCode);

/** Запуск подсчета времени
*/
void STM32ProfileStart(void);

/** Завершение процесса подсчета времени
	\return Подсчитанное время в выбранном масштабе 
*/
uint32_t STM32ProfileStop(void);

#endif
/**  
  @}
*/