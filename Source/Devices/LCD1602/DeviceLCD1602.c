#include <stdarg.h>
#include <string.h>

#include "DeviceLCD1602.h"
#include "jhal_environment.h"
#include "jhal_tick.h"

#define LCD1602_INSTRUCTION_REG         0x0
#define LCD1602_DATA_REG                0x1

#define LCD1602_READ_MODE               0x1
#define LCD1602_WRITE_MODE              0x0

#define LCD1602_DATA_CLEAR_DISPLAY      0x1
#define LCD1602_DATA_RETURN_HOME        0x2

#define TIME_FOR_DELAY                  5

#define DEFAULT_TIMEOUT                 5000

static uint8_t prvLCD1602WriteInstruction(DeviceLCD1602* pInstance, uint8_t Instruction)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  pInstance->ExtCode = LCD1602_NO_CODES;
  
  uint8_t res = pInstance->FuncReadWrite(pInstance->pInstanceDrv, pInstance->pParams, 
                                         &Instruction, LCD1602_INSTRUCTION_REG, LCD1602_WRITE_MODE);
  
  if(res != JHAL_RES_NO_ERRORS)
    pInstance->ExtCode = LCD1602_INTERFACE_ERROR;
  
  return res;
  
}

static uint8_t prvLCD1602ReadBusyFlagAndAddr(DeviceLCD1602* pInstance, uint8_t* pBusyFlag, uint8_t* pAddressCounter)
{
  if(!pInstance || (!pBusyFlag && !pAddressCounter))
    return JHAL_RES_INVALID_PARAMS;

  pInstance->ExtCode = LCD1602_NO_CODES;
  
  uint8_t ReadData;
  uint8_t res = pInstance->FuncReadWrite(pInstance->pInstanceDrv, pInstance->pParams, 
                                         &ReadData, LCD1602_INSTRUCTION_REG, LCD1602_READ_MODE);
  
  if(res != JHAL_RES_NO_ERRORS)
  {
    pInstance->ExtCode = LCD1602_INTERFACE_ERROR;
    return res;
  }
  
  if(pBusyFlag)
    *pBusyFlag = (ReadData >> 7) & 0x1;
  
  if(pAddressCounter)
    *pAddressCounter = ReadData & 0x7F;
  
  return JHAL_RES_NO_ERRORS;
}

static uint8_t prvLCD1602WriteData(DeviceLCD1602* pInstance, uint8_t Data)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  pInstance->ExtCode = LCD1602_NO_CODES;
  
  uint8_t res = pInstance->FuncReadWrite(pInstance->pInstanceDrv, pInstance->pParams, 
                                         &Data, LCD1602_DATA_REG, LCD1602_WRITE_MODE);
  
  if(res != JHAL_RES_NO_ERRORS)
    pInstance->ExtCode = LCD1602_INTERFACE_ERROR;  
  
  return res;
}

static uint8_t prvLCD1602ReadData(DeviceLCD1602* pInstance, uint8_t* pData)
{
  if(!pInstance || !pData)
    return JHAL_RES_INVALID_PARAMS;
  
  pInstance->ExtCode = LCD1602_NO_CODES;  
  
  uint8_t res = pInstance->FuncReadWrite(pInstance->pInstanceDrv, pInstance->pParams, 
                                         pData, LCD1602_DATA_REG, LCD1602_READ_MODE);
  
  if(res != JHAL_RES_NO_ERRORS)
    pInstance->ExtCode = LCD1602_INTERFACE_ERROR;  
  
  return res;  
}

static uint8_t prvLCD1602WaitPerformed(DeviceLCD1602* pInstance, uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;  
  
  uint8_t Res = JHAL_RES_NO_ERRORS;
  
  if(timeout != LCD1602_NO_TIMEOUT)
  {
    uint8_t BusyFlag;
    int32_t TimeCounter = (int32_t)timeout;
    
    while(1)    
    {
      jhal_tick(TIME_FOR_DELAY);    
      prvLCD1602ReadBusyFlagAndAddr(pInstance, &BusyFlag, NULL);
      
      if(!BusyFlag)
      {
        Res = JHAL_RES_NO_ERRORS;
        break;
      }
        
      TimeCounter -= TIME_FOR_DELAY;
      if(TimeCounter <= 0)
      {
        Res = JHAL_RES_TIMEOUT;
        break;        
      }
    }        
  }
  
  return Res;
}

uint8_t LCD1602ClearDisplay(DeviceLCD1602* pInstance, uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, LCD1602_DATA_CLEAR_DISPLAY);
  
  if(res == JHAL_RES_NO_ERRORS)
    return prvLCD1602WaitPerformed(pInstance, timeout);
  
  return res;
}

uint8_t LCD1602ReturnHome(DeviceLCD1602* pInstance, uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, LCD1602_DATA_RETURN_HOME);

  if(res == JHAL_RES_NO_ERRORS)
    return prvLCD1602WaitPerformed(pInstance, timeout);
    
  return res;
}

uint8_t LCD1602EntryModeSet(DeviceLCD1602* pInstance, LCD1602TypeMoveCursor Cursor,
                            LCD1602ShiftEntireDisplay ShiftEntire, uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t WriteData = ((uint8_t)Cursor << 1)|((uint8_t)ShiftEntire)|0x4;
  uint8_t res = prvLCD1602WriteInstruction(pInstance, WriteData);

  if(res == JHAL_RES_NO_ERRORS)
  {
    res = prvLCD1602WaitPerformed(pInstance, timeout);
    
    if(res == JHAL_RES_NO_ERRORS)
    {
      pInstance->Settings.TypeMoveCursor = Cursor;
      pInstance->Settings.ShiftEntireDisplay = ShiftEntire;
    }
  }
    
  return res;
}

uint8_t LCD1602DisplayOnOff(DeviceLCD1602* pInstance, LCD1602DisplayState DisplayState,
                            LCD1602CursorState CursorState, LCD1602CursorBlinkState CursorBlinkState,
                            uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t WriteData = ((uint8_t)DisplayState << 2)|((uint8_t)CursorState << 1)|
                      ((uint8_t)CursorBlinkState)|0x8;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, WriteData);

  if(res == JHAL_RES_NO_ERRORS)
  {
    res = prvLCD1602WaitPerformed(pInstance, timeout);
    
    if(res == JHAL_RES_NO_ERRORS)
    {
      pInstance->Settings.DisplayState = DisplayState;
      pInstance->Settings.CursorState = CursorState;
      pInstance->Settings.BlinkState = CursorBlinkState;
    }
  }
  
  return res;  
}

uint8_t LCD1602CursorDisplayShift(DeviceLCD1602* pInstance, LCD1602TypeShift Shift,
                                  uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t WriteData = ((uint8_t)Shift << 2)|0x10;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, WriteData);

  if(res == JHAL_RES_NO_ERRORS)
  {
    res = prvLCD1602WaitPerformed(pInstance, timeout);
    
    if(res == JHAL_RES_NO_ERRORS)
      pInstance->Settings.TypeShift = Shift;
  }
  
  return res;    
}


uint8_t LCD1602FunctionSet(DeviceLCD1602* pInstance, LCD1602TypeInterface Interface,
                            LCD1602NumberLine NumberLine, LCD1602FontSize FontSize,
                            uint32_t timeout)
{
  if(!pInstance)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t WriteData = ((uint8_t)Interface << 4)|((uint8_t)NumberLine << 3)|
                      ((uint8_t)FontSize << 2)|0x20;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, WriteData);

  if(res == JHAL_RES_NO_ERRORS)
  {
    res = prvLCD1602WaitPerformed(pInstance, timeout);
    
    if(res == JHAL_RES_NO_ERRORS)
    {
      pInstance->Settings.TypeInterface = Interface;
      pInstance->Settings.NumberLine = NumberLine;
      pInstance->Settings.FontSize = FontSize;
    }
  }
    
  return res;  
}

uint8_t LCD1602SetCGRAMAddress(DeviceLCD1602* pInstance, uint8_t Address, uint32_t timeout)
{
  if(!pInstance || Address > MAX_CGRAM_ADDRESS)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t WriteData = Address | 0x40;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, WriteData);

  if(res == JHAL_RES_NO_ERRORS)
    return prvLCD1602WaitPerformed(pInstance, timeout);
    
  return res;
}

uint8_t LCD1602SetDDRAMAddress(DeviceLCD1602* pInstance, uint8_t Address, uint32_t timeout)
{
  if(!pInstance || Address > MAX_DDRAM_ADDRESS)
    return JHAL_RES_INVALID_PARAMS;
  
  uint8_t WriteData = Address | 0x80;
  
  uint8_t res = prvLCD1602WriteInstruction(pInstance, WriteData);

  if(res == JHAL_RES_NO_ERRORS)
    return prvLCD1602WaitPerformed(pInstance, timeout);
    
  return res;
}

uint8_t LCD1602Printf(DeviceLCD1602* pInstance, const char* line1, const char* line2, ...)
{
    va_list args;
    
    if(!pInstance || !line1 || !line2)
      return JHAL_RES_INVALID_PARAMS;
    
    if(pInstance->Settings.NumberLine != LCD1602_NUMBER_LINE_2)
      return JHAL_RES_NOT_SUPPORTED;
    
    va_start(args, line2);
    
    char FirstLine[LCD1602_LENGTH_LINE + 1];
    if(vsnprintf(FirstLine, LCD1602_LENGTH_LINE + 1, line1, args) < 0)
    {
      va_end(args);
      return JHAL_RES_ERROR;
    }
    
    char SecondLine[LCD1602_LENGTH_LINE + 1];
    if(vsnprintf(SecondLine, LCD1602_LENGTH_LINE + 1, line2, args) < 0)
    {
      va_end(args);
      return JHAL_RES_ERROR;
    }
      
    va_end(args);
    
    uint8_t res = LCD1602EntryModeSet(pInstance, LCD1602_CURSOR_MOVE_RIGHT, 
                        LCD1602_SHIFT_ENTIRE_NOT_PERFORMED, DEFAULT_TIMEOUT);
    
    if(res != JHAL_RES_NO_ERRORS)
      return res;
    
    res = LCD1602ClearDisplay(pInstance, DEFAULT_TIMEOUT);
    
    if(res != JHAL_RES_NO_ERRORS)
      return res;    
    
    res = LCD1602SetDDRAMAddress(pInstance, LCD1602_DDRAM_START_LINE1, DEFAULT_TIMEOUT);
    
    if(res != JHAL_RES_NO_ERRORS)
      return res;
    
    for(uint8_t i = 0; i < strlen(FirstLine); i++)
    {
        prvLCD1602WriteData(pInstance, (uint8_t)FirstLine[i]);      
        prvLCD1602WaitPerformed(pInstance, DEFAULT_TIMEOUT);
    }
      
    res = LCD1602SetDDRAMAddress(pInstance, LCD1602_DDRAM_START_LINE2, DEFAULT_TIMEOUT);
    
    if(res != JHAL_RES_NO_ERRORS)
      return res;
    
    for(uint8_t i = 0; i < strlen(SecondLine); i++)
    {
        prvLCD1602WriteData(pInstance, (uint8_t)SecondLine[i]);      
        prvLCD1602WaitPerformed(pInstance, DEFAULT_TIMEOUT);
    }    
    
    return res;
}