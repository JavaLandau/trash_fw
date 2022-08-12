#ifndef __JHAL_UART__
#define __JHAL_UART__

#ifdef __cplusplus
extern "C" {
#endif

#include "jhal_environment.h"

typedef void (*jhal_type_uart_tx_complete)(void*);
typedef void (*jhal_type_uart_txrx_complete)(void*, uint8_t* prxdata, uint16_t size);
typedef void (*jhal_type_uart_rx_complete)(void*, uint8_t* prxdata, uint16_t size);
  
typedef enum {
  JHAL_UART_BAUDRATE_300    = 123U,
  JHAL_UART_BAUDRATE_600    = 14U,
  JHAL_UART_BAUDRATE_1200   = 208U,
  JHAL_UART_BAUDRATE_2400   = 211U,
  JHAL_UART_BAUDRATE_4800   = 105U,
  JHAL_UART_BAUDRATE_9600   = 175U,
  JHAL_UART_BAUDRATE_19200  = 76U,
  JHAL_UART_BAUDRATE_38400  = 43U,
  JHAL_UART_BAUDRATE_57600  = 102U,
  JHAL_UART_BAUDRATE_115200 = 66U,
  JHAL_UART_BAUDRATE_230400 = 222U,
  JHAL_UART_BAUDRATE_460800 = 117U,
  JHAL_UART_BAUDRATE_921600 = 217U
} jhal_uart_baudrate;

typedef enum {
  JHAL_UART_DATA_SIZE_5BIT = 35U,
  JHAL_UART_DATA_SIZE_6BIT = 124U,
  JHAL_UART_DATA_SIZE_7BIT = 233U,
  JHAL_UART_DATA_SIZE_8BIT = 67U,
  JHAL_UART_DATA_SIZE_9BIT = 55U,
} jhal_uart_data_size;

typedef enum {
  JHAL_UART_STOP_BITS_1BIT  = 89U,
  JHAL_UART_STOP_BITS_15BIT = 177U,
  JHAL_UART_STOP_BITS_2BIT  = 165U
} jhal_uart_stop_bits;

typedef enum {
  JHAL_UART_PARITY_NONE  = 90U,
  JHAL_UART_PARITY_EVEN  = 208U,
  JHAL_UART_PARITY_ODD   = 22U
} jhal_uart_parity;

typedef enum {
  JHAL_UART_HWR_FLOW_CTRL_NOT_USE = 33U,
  JHAL_UART_HWR_FLOW_CTRL_USE = 67U
} jhal_uart_hwr_flow_ctrl;

typedef struct {
  uint8_t                        num_module;
  jhal_uart_baudrate             baudrate;
  jhal_uart_data_size            data_size;
  jhal_uart_stop_bits            stop_bits;
  jhal_uart_parity               parity;
  jhal_uart_hwr_flow_ctrl        hwr_flow_ctrl;  
  
  jhal_type_uart_tx_complete     pfunc_tx_complete;
  jhal_type_uart_rx_complete     pfunc_rx_complete;
  jhal_type_uart_txrx_complete   pfunc_txrx_complete;  
  void*                          plib_data;
  void*                          puser_data;
} jhal_uart_params;

uint8_t jhal_uart_init(void** ppinstance, jhal_uart_params* pparams);
uint8_t jhal_uart_deinit(void* pinstance);
uint8_t jhal_uart_transmit(void* pinstance, uint8_t* ptxdata, uint16_t size, uint32_t timeout);
uint8_t jhal_uart_receive(void* pinstance, uint8_t* prxdata, uint16_t size, uint32_t timeout);
uint8_t jhal_uart_transmitreceive(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, uint32_t timeout);
uint8_t jhal_uart_transmit_it(void* pinstance, uint8_t* ptxdata, uint16_t size);
uint8_t jhal_uart_receive_it(void* pinstance, uint8_t* prxdata, uint16_t size);
uint8_t jhal_uart_transmitreceive_it(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size);
uint8_t jhal_uart_transmit_dma(void* pinstance, uint8_t* ptxdata, uint16_t size, void* pinstance_dma);
uint8_t jhal_uart_receive_dma(void* pinstance, uint8_t* prxdata, uint16_t size, void* pinstance_dma);
uint8_t jhal_uart_transmitreceive_dma(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, void* pinstance_dma);

void jhal_uart_tx_complete_callback(void* pinstance);
void jhal_uart_rx_complete_callback(void* pinstance, uint8_t* prxdata, uint16_t size);
void jhal_uart_txrx_complete_callback(void* pinstance, uint8_t* prxdata, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif