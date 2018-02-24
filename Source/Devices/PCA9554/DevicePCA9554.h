/**
\file
  \brief Заголовочный файл расширителя портов GPIO PCA9554
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017  
*/

#ifndef __DEVICE_PCA9554
#define __DEVICE_PCA9554

#include "stm32f0xx_hal.h"

/**
  \addtogroup module_PCA9554
@{
*/

#define PORT_PIN_INPUT                  0x1	        ///<Порт GPIO типа "вход"
#define PORT_PIN_OUTPUT                 0x0             ///<Порт GPIO типа "выход"

#define PORT_PIN_POLARITY_RETAINED      0x0             ///<Порт GPIO неинвертированный
#define PORT_PIN_POLARITY_INVERTED      0x1             ///<Порт GPIO инвертированный

#define PCA9554_NUM_GPIO                8               ///<Число портов GPIO в PCA9554

///Структура драйвера
typedef struct _DevicePCA9554 {
  I2C_HandleTypeDef I2CDrv;                             ///<Указатель на структуру драйвера \f$I^2C\f$
  
  GPIO_TypeDef*     pGPIOINTPort;                       ///<Указатель на структуру GPIO для сигнала прерывания INT
  uint16_t          GPIOINTNum;                         ///<Указатель на номер GPIO для сигнала прерывания INT
  uint8_t           DevAddress;                         ///<Адрес \f$I^2C\f$ микросхемы
  uint8_t           ConfRegister;                       ///<Копия конфигурационного регистра PCA9554
  uint8_t           OutputPortRegister;                 ///<Копия регистра выходов PCA9554   
} DevicePCA9554;

///Структура параметров инициализации драйвера
typedef struct _DevicePCA9554Param {
  I2C_TypeDef*      pInstanceI2CDrv;                    ///<Указатель на параметры драйвера \f$I^2C\f$     
  uint8_t           AddrDevice;                         ///<Младший адрес \f$I^2C\f$ микросхемы
  char*             GPIOINTPin;                         ///<Строка описания номера пина микроконтроллера, используемого для сигнала INT
} DevicePCA9554Param;

///Специальные коды ошибок
typedef enum _ExtCodeDevicePCA9554 {
  DEVICE_PCA9554_NOT_CODE = 0,                          ///<Ошибок нет
  DEVICE_PCA9554_ERROR_INIT_I2C,                        ///<Ошибка инициализации \f$I^2C\f$
  DEVICE_PCA9554_ERROR_INIT_GPIO,                       ///<Ошибка инициализации GPIO
  DEVICE_PCA9554_ERROR_PROCESS_I2C,                     ///<Ошибка при работе с \f$I^2C\f$
  DEVICE_PCA9554_ERROR_WRITE_I2C                        ///<Ошибка записи данных в микросхему
} ExtCodeDevicePCA9554;

/**Инициализация драйвера PCA9554
  \param[in] pDevParam указатель на структуру параметров для инициализации драйвера
  \param[out] pDev указатель на структуру драйвера
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554Create(DevicePCA9554Param* pDevParam, DevicePCA9554* pDev, ExtCodeDevicePCA9554* pExCode);

/**Считывание значений всех входов GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[out] pPortState состояние входов GPIO 
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554GetAllInputPorts(DevicePCA9554* pDev, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode);

/**Считывание значения отдельного входа GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[in] NumPort номер порта GPIO 
  \param[out] pPortState состояние входа GPIO 
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554GetInputPort(DevicePCA9554* pDev, uint8_t NumPort, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode);

/**Установка значений всех выходов GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[in] pPortState значения выходов GPIO 
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554SetAllOutputPorts(DevicePCA9554* pDev, GPIO_PinState* pPortState, ExtCodeDevicePCA9554* pExCode);

/**Установка значения отдельного выхода GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[in] NumPort номер порта GPIO 
  \param[in] PortState значение выхода GPIO 
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554SetOutputPort(DevicePCA9554* pDev, uint8_t NumPort, GPIO_PinState PortState, ExtCodeDevicePCA9554* pExCode);

/**Конфигурирование всех портов GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[in] pPortDirection направления портов GPIO 
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554SetConfRegister(DevicePCA9554* pDev, uint8_t* pPortDirection, ExtCodeDevicePCA9554* pExCode);


/**Конфигурирование отдельных портов GPIO
  \param[in] pDev указатель на структуру драйвера
  \param[in] NumPort номер порта GPIO
  \param[in] PortDirection направление порта GPIO
  \param[out] pExCode специальный код ошибки выполнения функции
  \return Результат выполнения функции 
*/
uint32_t DevicePCA9554SetConfPort(DevicePCA9554* pDev, uint8_t NumPort, uint8_t PortDirection, ExtCodeDevicePCA9554* pExCode);

/**
@}
*/

#endif