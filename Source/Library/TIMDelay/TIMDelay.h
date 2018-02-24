/**
  \file    TIMDelay.h 
  \brief   Заголовочный файл таймера задержки
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef _TIMDELAY_
#define _TIMDELAY_

/**Задержка на заданное время
  \param[in] microsec время задержки в микросекундах
*/
void TIMDelay(uint32_t microsec);

/**Инициализация таймера задержки
  \param[in] period период счетчика таймера TIM1
  \return Результат выполнения функции
*/
uint32_t InitTIM(uint32_t period);

#endif