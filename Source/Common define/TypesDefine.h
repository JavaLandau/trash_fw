/**
  \file    TypesDefine.h 
  \brief   Заголовочный файл определений типов переменных, кодов ошибок и другое
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __FUNC_DEFINE__
#define __FUNC_DEFINE__

/** @name Коды ошибок функций
*/                        
///@{
#define FUNC_OK                 0               ///<Ошибок нет
#define FUNC_ERROR              1               ///<Неизвестная ошибка
#define FUNC_INVALID_PARAM      2               ///<Неправильный формат входных данных
#define FUNC_HEAP_MEM_ERROR     3               ///<Ошибка выделения памяти в куче
///@}

/** @name Определение булевых значений
*/                        
///@{
#define FALSE                   0               ///<Логическая "ложь"
#define TRUE                    1               ///<Логическая "истина"
///@}

typedef void (*HwrIntFunc)(void*);              ///<Указатель на функцию-обработчика прерываний

#endif