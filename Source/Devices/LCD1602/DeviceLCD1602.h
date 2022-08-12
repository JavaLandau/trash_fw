#ifndef __DEVICE_LCD1602__
#define __DEVICE_LCD1602__

#include <stdint.h>

#define MAX_CGRAM_ADDRESS         0x3F
#define MAX_DDRAM_ADDRESS         0x7F

#define LCD1602_NO_TIMEOUT        0

#define LCD1602_DISABLE           0
#define LCD1602_ENABLE            1

#define LCD1602_LINE_1            0
#define LCD1602_LINE_2            1

#define LCD1602_NUMBER_OF_LINE    2

#define LCD1602_LENGTH_LINE       16

#define LCD1602_DDRAM_START_LINE  0x00
#define LCD1602_DDRAM_END_LINE    0x4F
#define LCD1602_DDRAM_START_LINE1 0x00
#define LCD1602_DDRAM_END_LINE1   0x27
#define LCD1602_DDRAM_START_LINE2 0x40
#define LCD1602_DDRAM_END_LINE2   0x67

typedef uint8_t (*LCD1602FuncInit)(void**, void*);
typedef uint8_t (*LCD1602FuncDeinit)(void*);
typedef uint8_t (*LCD1602FuncReadWrite)(void*, void*, uint8_t*, uint8_t, uint8_t);
typedef uint8_t (*LCD1602FuncEnableDisable)(void*, void*, uint8_t);

typedef enum {
  LCD1602_SHIFT_CURSOR_LEFT = 0,
  LCD1602_SHIFT_CURSOR_RIGHT = 1,
  LCD1602_SHIFT_DISPLAY_LEFT = 2,
  LCD1602_SHIFT_DISPLAY_RIGHT = 3
}LCD1602TypeShift;

typedef enum {
  LCD1602_NUMBER_LINE_1 = 0,
  LCD1602_NUMBER_LINE_2 = 1
} LCD1602NumberLine;

typedef enum {
  LCD1602_FONT_SIZE_5X8 = 0,
  LCD1602_FONT_SIZE_5X11 = 1
} LCD1602FontSize;

typedef enum {
  LCD1602_CURSOR_MOVE_LEFT  =    0,
  LCD1602_CURSOR_MOVE_RIGHT  =   1
} LCD1602TypeMoveCursor;

typedef enum {
  LCD1602_SHIFT_ENTIRE_NOT_PERFORMED  =    1,
  LCD1602_SHIFT_ENTIRE_PERFORMED  =   0
} LCD1602ShiftEntireDisplay;

typedef enum {
  LCD1602_4BITS_INTERFACE  =  0,
  LCD1602_8BITS_INTERFACE  =  1
} LCD1602TypeInterface;

typedef enum {
  LCD1602_DISPLAY_OFF  =  0,
  LCD1602_DISPLAY_ON   =  1
} LCD1602DisplayState;

typedef enum {
  LCD1602_CURSOR_OFF  =  0,
  LCD1602_CURSOR_ON   =  1
} LCD1602CursorState;

typedef enum {
  LCD1602_CURSOR_BLINK_OFF  =  0,
  LCD1602_CURSOR_BLINK_ON   =  1
} LCD1602CursorBlinkState;

typedef struct {
  LCD1602TypeShift              TypeShift;
  LCD1602NumberLine             NumberLine;
  LCD1602FontSize               FontSize;
  LCD1602TypeMoveCursor         TypeMoveCursor;
  LCD1602ShiftEntireDisplay     ShiftEntireDisplay;
  LCD1602TypeInterface          TypeInterface;
  LCD1602DisplayState           DisplayState;
  LCD1602CursorState            CursorState;
  LCD1602CursorBlinkState       BlinkState;
} LCD1602Settings;

typedef enum {
  LCD1602_NO_CODES              = 0,
  LCD1602_NO_CONNECTION,
  LCD1602_INTERFACE_ERROR,
} LCD1602ExtCode;

typedef struct { 
  void*                       pParams;
  LCD1602TypeInterface        TypeInterface;
  
  LCD1602FuncInit             FuncInit;
  LCD1602FuncDeinit           FuncDeinit;
  LCD1602FuncReadWrite        FuncReadWrite;
  LCD1602FuncEnableDisable    FuncEnableDisable;
} LCD1602Param;

typedef struct {
  void*                       pInstanceDrv;
  void*                       pParams;
  LCD1602FuncInit             FuncInit;
  LCD1602FuncDeinit           FuncDeinit;
  LCD1602FuncReadWrite        FuncReadWrite;
  LCD1602FuncEnableDisable    FuncEnableDisable;
  
  LCD1602Settings             Settings;
  
  LCD1602ExtCode              ExtCode;  
} DeviceLCD1602;

uint8_t LCD1602Printf(DeviceLCD1602* pInstance, const char* line1, const char* line2, ...);
uint8_t LCD1602SetDDRAMAddress(DeviceLCD1602* pInstance, uint8_t Address, uint32_t timeout);
uint8_t LCD1602SetCGRAMAddress(DeviceLCD1602* pInstance, uint8_t Address, uint32_t timeout);

uint8_t LCD1602FunctionSet(DeviceLCD1602* pInstance, LCD1602TypeInterface Interface,
                            LCD1602NumberLine NumberLine, LCD1602FontSize FontSize,
                            uint32_t timeout);

uint8_t LCD1602CursorDisplayShift(DeviceLCD1602* pInstance, LCD1602TypeShift Shift,
                                  uint32_t timeout);

uint8_t LCD1602DisplayOnOff(DeviceLCD1602* pInstance, LCD1602DisplayState DisplayState,
                            LCD1602CursorState CursorState, LCD1602CursorBlinkState CursorBlinkState,
                            uint32_t timeout);

uint8_t LCD1602EntryModeSet(DeviceLCD1602* pInstance, LCD1602TypeMoveCursor Cursor,
                            LCD1602ShiftEntireDisplay ShiftEntire, uint32_t timeout);

uint8_t LCD1602ReturnHome(DeviceLCD1602* pInstance, uint32_t timeout);
uint8_t LCD1602ClearDisplay(DeviceLCD1602* pInstance, uint32_t timeout);
#endif
