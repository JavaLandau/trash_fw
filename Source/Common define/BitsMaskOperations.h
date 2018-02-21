/**
  \file    BitsMaskOperations.h 
  \brief   Заголовочный файл макросов битовых маскированных операций для общего применения
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __BITS_MASK_OPERATIONS__
#define __BITS_MASK_OPERATIONS__

#define RESET		       0        ///<Сброс бита/байта/регистра
#define SET		       1        ///<Установка бита/байта/регистра

/**
  \defgroup module_bits_op_4_var Битовые маскированные операции для переменных
  \brief Модуль битовых маскированных операций для работы с переменными
@{
*/

///Сброс/установка заданного бита в заданном байте
#define CHANGE_BIT(SRC,NUM,VALUE)    if(VALUE) (SRC)|=(char)(1<<(NUM));\
                                     else (SRC)&=(char)(~(1<<(NUM)));
///Сброс заданного бита в заданном байте
#define RESET_BIT(SRC,NUM)           ((SRC)&=(char)(~(1<<(NUM))))

///Установка заданного бита в заданном байте
#define SET_BIT(SRC,NUM)   	     ((SRC)|=(char)(1<<(NUM))

///Значение заданного бита в заданном байте
#define GET_BIT(SRC, NUM)            ((char)(((SRC)>>(NUM))&0xFF))
/**  
  @}
*/

/**
  \defgroup module_bits_op_4_reg Битовые маскированные операции для регистров
  \brief Модуль битовых маскированных операций для работы с регистрами процессора или указателями
@{
*/

///Сброс/установка заданного бита в заданном регистре
#define CHANGE_REG_BIT(SRC,NUM,VALUE) if(VALUE) (*SRC)|=(char)(1<<(NUM));\
                                      else (*SRC)&=(char)(~(1<<(NUM)));

///Сброс заданного бита в заданном регистре                                      
#define RESET_REG_BIT(SRC,NUM)        ((*SRC)&=(char)(~(1<<(NUM))))

///Установка заданного бита в заданном регистре                                      
#define SET_REG_BIT(SRC,NUM)          ((*SRC)|=(char)(1<<(NUM)))

///Значение заданного бита в заданном регистре                                      
#define GET_REG_BIT(SRC, NUM)         ((char)(((*SRC)>>(NUM))&0xFF))

///Изменение заданного регистра на заданное значение                                         
#define CHANGE_REG(SRC,VALUE)	      (*(SRC) = (VALUE))

///Значение заданного регистра                                      
#define GET_REG(SRC)	              (*(SRC))

///Установка заданного регистра                                      
#define SET_REG(SRC)                  (*(SRC) = 0xFF)  

///Сброс заданного регистра                                      
#define RESET_REG(SRC)                (*(SRC) = 0x00)  
/**  
  @}
*/
                                      
#endif