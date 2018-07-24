/**
  \file    TypesDefine.h 
  \brief   Header file of types of variables definition, error codes && etc.
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

#ifndef __FUNC_DEFINE__
#define __FUNC_DEFINE__

/** @name Error codes for functions
*/                        
///@{
#define FUNC_OK                 0               ///<No error
#define FUNC_ERROR              1               ///<Unknown error
#define FUNC_INVALID_PARAM      2               ///<Invalid format of input arguments
#define FUNC_HEAP_MEM_ERROR     3               ///<Error of allocate memory
///@}

/** @name Bool types definition
*/                        
///@{
#define FALSE                   0               ///<Logical "false"
#define TRUE                    1               ///<Logical "true"
///@}

typedef void (*HwrIntFunc)(void*);              ///<Pointer to interrupt callback

#endif