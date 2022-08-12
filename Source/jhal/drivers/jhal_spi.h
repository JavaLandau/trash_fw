#ifndef __JHAL_SPI__
#define __JHAL_SPI__

#ifdef __cplusplus
extern "C" {
#endif

#include "jhal_environment.h"

typedef void (*jhal_type_spi_tx_complete)(void*);
typedef void (*jhal_type_spi_txrx_complete)(void*, uint8_t* prxdata, uint16_t size);
typedef void (*jhal_type_spi_rx_complete)(void*, uint8_t* prxdata, uint16_t size);
  
typedef enum {
  JHAL_SPI_MODE_MASTER = 37U,
  JHAL_SPI_MODE_SLAVE = 22U
} jhal_spi_mode;

typedef enum {
  JHAL_SPI_FIRST_BIT_LSB = 141U,
  JHAL_SPI_FIRST_BIT_MSB = 67U
} jhal_spi_first_bit;

typedef enum {
  JHAL_SPI_DATA_SIZE_8BIT  = 209U,
  JHAL_SPI_DATA_SIZE_16BIT = 108U
} jhal_spi_data_size;

typedef enum {
  JHAL_SPI_CPOL_LOW  = 249U,
  JHAL_SPI_CPOL_HIGH = 117U
} jhal_spi_cpol;

typedef enum {
  JHAL_SPI_CPHA_LOW  = 30U,
  JHAL_SPI_CPHA_HIGH = 76U
} jhal_spi_cpha;

typedef struct {
  uint8_t                       num_module;
  jhal_spi_mode                 mode;
  jhal_spi_data_size            data_size;
  jhal_spi_cpol                 cpol;
  jhal_spi_cpha                 cpha;
  jhal_spi_first_bit            first_bit;
  uint32_t                      baudrate;
        
  jhal_type_spi_tx_complete     pfunc_tx_complete;
  jhal_type_spi_rx_complete     pfunc_rx_complete;
  jhal_type_spi_txrx_complete   pfunc_txrx_complete;  
  void*                         plib_data;
  void*                         puser_data;
} jhal_spi_params;


uint8_t jhal_spi_init(void** ppinstance, jhal_spi_params* pparams_spi);
uint8_t jhal_spi_deinit(void* pinstance);
uint8_t jhal_spi_transmit(void* pinstance, uint8_t* ptxdata, uint16_t size, uint32_t timeout);
uint8_t jhal_spi_receive(void* pinstance, uint8_t* prxdata, uint16_t size, uint32_t timeout);
uint8_t jhal_spi_transmitreceive(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, uint32_t timeout);
uint8_t jhal_spi_transmit_it(void* pinstance, uint8_t* ptxdata, uint16_t size);
uint8_t jhal_spi_receive_it(void* pinstance, uint8_t* prxdata, uint16_t size);
uint8_t jhal_spi_transmitreceive_it(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size);
uint8_t jhal_spi_transmit_dma(void* pinstance, uint8_t* ptxdata, uint16_t size, void* pinstance_dma);
uint8_t jhal_spi_receive_dma(void* pinstance, uint8_t* prxdata, uint16_t size, void* pinstance_dma);
uint8_t jhal_spi_transmitreceive_dma(void* pinstance, uint8_t* ptxdata, uint8_t* prxdata, uint16_t size, void* pinstance_dma);

void jhal_spi_tx_complete_callback(void* pinstance);
void jhal_spi_rx_complete_callback(void* pinstance, uint8_t* prxdata, uint16_t size);
void jhal_spi_txrx_complete_callback(void* pinstance, uint8_t* prxdata, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif