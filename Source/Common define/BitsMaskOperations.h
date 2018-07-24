/**
  \file    BitsMaskOperations.h 
  \brief   Header file of macros of masked operations for common purpose
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __BITS_MASK_OPERATIONS__
#define __BITS_MASK_OPERATIONS__

#define RESET		       0        ///<Reset bit/byte/register
#define SET		       1        ///<Set bit/byte/register

/**
  \defgroup module_bits_op_4_var Masked bit operations for variables
  \brief Module of masked bit operations for variables
@{
*/

///Set/reset preassigned bit in preassigned byte
#define CHANGE_BIT(SRC,NUM,VALUE)    if(VALUE) (SRC)|=(char)(1<<(NUM));\
                                     else (SRC)&=(char)(~(1<<(NUM)));
///Reset preassigned bit in preassigned byte
#define RESET_BIT(SRC,NUM)           ((SRC)&=(char)(~(1<<(NUM))))

///Set preassigned bit in preassigned byte
#define SET_BIT(SRC,NUM)   	     ((SRC)|=(char)(1<<(NUM))

///Value of preassigned bit in preassigned byte
#define GET_BIT(SRC, NUM)            ((char)(((SRC)>>(NUM))&0xFF))
/**
  @}
*/

/**
  \defgroup module_bits_op_4_reg Masked bit operations for registers
  \brief Module of masked bit operations for registers
@{
*/

///Set/reset preassigned bit in preassigned register
#define CHANGE_REG_BIT(SRC,NUM,VALUE) if(VALUE) (*SRC)|=(char)(1<<(NUM));\
                                      else (*SRC)&=(char)(~(1<<(NUM)));

///Reset preassigned bit in preassigned register
#define RESET_REG_BIT(SRC,NUM)        ((*SRC)&=(char)(~(1<<(NUM))))

///Set preassigned bit in preassigned register
#define SET_REG_BIT(SRC,NUM)          ((*SRC)|=(char)(1<<(NUM)))

///Value of preassigned bit in preassigned register
#define GET_REG_BIT(SRC, NUM)         ((char)(((*SRC)>>(NUM))&0xFF))

///Change value of register
#define CHANGE_REG(SRC,VALUE)	      (*(SRC) = (VALUE))

///Value of register
#define GET_REG(SRC)	              (*(SRC))

///Set register
#define SET_REG(SRC)                  (*(SRC) = 0xFF)

///Reset register
#define RESET_REG(SRC)                (*(SRC) = 0x00)
/**
  @}
*/
                                      
#endif