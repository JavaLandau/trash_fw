/**
  \file    DeviceCC1200Defs.h
  \brief   Заголовочный файл определений и констант для драйвера приёмопередатчика CC1200
  \author  JavaLandau
  \version 1.0
  \date    20.12.2017 
*/

/**
@}
  \addtogroup module_CC1200
@{
*/

/** @name Флаги состояний приёмопередатчика
*/                        
///@{
#define CHIP_STATUS_IDLE                0x0     ///<Ожидание
#define CHIP_STATUS_RX                  0x1     ///<Приём
#define CHIP_STATUS_TX                  0x2     ///<Передача
#define CHIP_STATUS_FSTXON              0x3     ///<Готовность "быстрой" передачи
#define CHIP_STATUS_CALIBRATE           0x4     ///<Калибровка синтезатора частоты
#define CHIP_STATUS_SETTLING            0x5     ///<Установка PLL
#define CHIP_STATUS_RX_FIFO_ERROR       0x6     ///<Ошибка буфера RX
#define CHIP_STATUS_TX_FIFO_ERROR       0x7     ///<Ошибка буфера TX
///@}

#define CHIP_READY                      0x0     ///<Флаг готовности приёмопередатчика

/** @name Адреса конфигурационных регистров
*/                        
///@{
#define IOCFG3                          0x00
#define IOCFG2                          0x01
#define IOCFG1                          0x02
#define IOCFG0                          0x03
#define SYNC3                           0x04
#define SYNC2                           0x05
#define SYNC1                           0x06
#define SYNC0                           0x07
#define SYNC_CFG1                       0x08
#define SYNC_CFG0                       0x09
#define DEVIATION_M                     0x0A
#define MODCFG_DEV_E                    0x0B
#define DCFILT_CFG                      0x0C
#define PREAMBLE_CFG1                   0x0D
#define PREAMBLE_CFG0                   0x0E
#define IQIC                            0x0F
#define CHAN_BW                         0x10
#define MDMCFG1                         0x11
#define MDMCFG0                         0x12
#define SYMBOL_RATE2                    0x13
#define SYMBOL_RATE1                    0x14
#define SYMBOL_RATE0                    0x15
#define AGC_REF                         0x16
#define AGC_CS_THR                      0x17
#define AGC_GAIN_ADJUST                 0x18
#define AGC_CFG3                        0x19
#define AGC_CFG2                        0x1A
#define AGC_CFG1                        0x1B
#define AGC_CFG0                        0x1C
#define FIFO_CFG                        0x1D
#define DEV_ADDR                        0x1E
#define SETTLING_CFG                    0x1F
#define FS_CFG                          0x20
#define WOR_CFG1                        0x21
#define WOR_CFG0                        0x22
#define WOR_EVENT0_MSB                  0x23
#define WOR_EVENT0_LSB                  0x24
#define RXDCM_TIME                      0x25
#define PKT_CFG2                        0x26
#define PKT_CFG1                        0x27
#define PKT_CFG0                        0x28
#define RFEND_CFG1                      0x29
#define RFEND_CFG0                      0x2A
#define PA_CFG1                         0x2B
#define PA_CFG0                         0x2C
#define ASK_CFG                         0x2D
#define PKT_LEN                         0x2E
#define EXTENDED_ADDRESS                0x2F
///@}

/** @name Типы команд для приёмопередатчика
*/                        
///@{
#define SRES                            0x30    ///<Перезагрузка
#define SFSTXON                         0x31    ///<Включение и калибровка синтезатора частоты
#define SXOFF                           0x32    ///<Отключение
#define SCAL                            0x33    ///<Калибровка и отключение синтезатора частоты
#define SRX                             0x34    ///<Режим приёма
#define STX                             0x35    ///<Режим передачи
#define SIDLE                           0x36    ///<Режим ожидания
#define SAFC                            0x37    ///<Частотная автокомпенсация 
#define SWOR                            0x38    ///<Включение режима eWOR
#define SPWD                            0x39    ///<Режим сна
#define SFRX                            0x3A    ///<Очистка буфера RX
#define SFTX                            0x3B    ///<Очистка буфера TX
#define SWORRST                         0x3C    ///<Перезапуск таймера eWOR по значению Event1
#define SNOP                            0x3D    ///<Пустая операция
#define DIRECT_MEMORY_ACCESS            0x3E    ///<Однобайтный режим доступа к памяти
#define STANDART_FIFO_ACCESS            0x3F    ///<Пакетный режим доступа к памяти
///@}

/** @name Адреса регистров из расширенной области памяти
*/                        
///@{
#define IF_MIX_CFG                      0x00
#define FREQOFF_CFG                     0x01
#define TOC_CFG                         0x02
#define MARC_SPARE                      0x03
#define ECG_CFG                         0x04
#define MDMCFG2                         0x05
#define EXT_CTRL                        0x06
#define RCCAL_FINE                      0x07
#define RCCAL_COARSE                    0x08
#define RCCAL_OFFSET                    0x09
#define FREQOFF1                        0x0A
#define FREQOFF0                        0x0B
#define FREQ2                           0x0C
#define FREQ1                           0x0D
#define FREQ0                           0x0E
#define IF_ADC2                         0x0F
#define IF_ADC1                         0x10
#define IF_ADC0                         0x11
#define FS_DIG1                         0x12
#define FS_DIG0                         0x13
#define FS_CAL3                         0x14
#define FS_CAL2                         0x15
#define FS_CAL1                         0x16
#define FS_CAL0                         0x17
#define FS_CHP                          0x18
#define FS_DIVTWO                       0x19
#define FS_DSM1                         0x1A
#define FS_DSM0                         0x1B
#define FS_DVC1                         0x1C
#define FS_DVC0                         0x1D
#define FS_LBI                          0x1E
#define FS_PFD                          0x1F
#define FS_PRE                          0x20
#define FS_REG_DIV_CML                  0x21
#define FS_SPARE                        0x22
#define FS_VCO4                         0x23
#define FS_VCO3                         0x24
#define FS_VCO2                         0x25
#define FS_VCO1                         0x26
#define FS_VCO0                         0x27
#define GBIAS6                          0x28
#define GBIAS5                          0x29
#define GBIAS4                          0x2A
#define GBIAS3                          0x2B
#define GBIAS2                          0x2C
#define GBIAS1                          0x2D
#define GBIAS0                          0x2E
#define IFAMP                           0x2F
#define LNA                             0x30
#define RXMIX                           0x31
#define XOSC5                           0x32
#define XOSC4                           0x33
#define XOSC3                           0x34
#define XOSC2                           0x35
#define XOSC1                           0x36
#define XOSC0                           0x37
#define ANALOG_SPARE                    0x38
#define PA_CFG3                         0x39

//0x3A - 0x3E Not Used
//0x3F - 0x40 Reserved
//0x41 - 0x63 Not Used

#define WOR_TIME1                       0x64
#define WOR_TIME0                       0x65
#define WOR_CAPTURE1                    0x66
#define WOR_CAPTURE0                    0x67
#define BIST                            0x68
#define DCFILTOFFSET_I1                 0x69
#define DCFILTOFFSET_I0                 0x6A
#define DCFILTOFFSET_Q1                 0x6B
#define DCFILTOFFSET_Q0                 0x6C
#define IQIE_I1                         0x6D
#define IQIE_I0                         0x6E
#define IQIE_Q1                         0x6F
#define IQIE_Q0                         0x70
#define RSSI1                           0x71
#define RSSI0                           0x72
#define MARCSTATE                       0x73
#define LQI_VAL                         0x74
#define PQT_SYNC_ERR                    0x75
#define DEM_STATUS                      0x76
#define FREQOFF_EST1                    0x77
#define FREQOFF_EST0                    0x78
#define AGC_GAIN3                       0x79
#define AGC_GAIN2                       0x7A
#define AGC_GAIN1                       0x7B
#define AGC_GAIN0                       0x7C
#define CFM_RX_DATA_OUT                 0x7D
#define CFM_TX_DATA_IN                  0x7E
#define ASK_SOFT_RX_DATA                0x7F
#define RNDGEN                          0x80
#define MAGN2                           0x81
#define MAGN1                           0x82
#define MAGN0                           0x83
#define ANG1                            0x84
#define ANG0                            0x85
#define CHFILT_I2                       0x86
#define CHFILT_I1                       0x87
#define CHFILT_I0                       0x88
#define CHFILT_Q2                       0x89
#define CHFILT_Q1                       0x8A
#define CHFILT_Q0                       0x8B
#define GPIO_STATUS                     0x8C
#define FSCAL_CTRL                      0x8D
#define PHASE_ADJUST                    0x8E
#define PARTNUMBER                      0x8F
#define PARTVERSION                     0x90
#define SERIAL_STATUS                   0x91
#define MODEM_STATUS1                   0x92
#define MODEM_STATUS0                   0x93
#define MARC_STATUS1                    0x94
#define MARC_STATUS0                    0x95
#define PA_IFAMP_TEST                   0x96
#define FSRF_TEST                       0x97
#define PRE_TEST                        0x98
#define PRE_OVR                         0x99
#define ADC_TEST                        0x9A
#define DVC_TEST                        0x9B
#define ATEST                           0x9C
#define ATEST_LVDS                      0x9D
#define ATEST_MODE                      0x9E
#define XOSC_TEST1                      0x9F
#define XOSC_TEST0                      0xA0
#define AES                             0xA1
#define MDM_TEST                        0xA2

//0xA3 - 0xD1 Not Used

#define RXFIRST                         0xD2
#define TXFIRST                         0xD3
#define RXLAST                          0xD4
#define TXLAST                          0xD5
#define NUM_TXBYTES                     0xD6
#define NUM_RXBYTES                     0xD7
#define FIFO_NUM_TXBYTES                0xD8
#define FIFO_NUM_RXBYTES                0xD9
#define RXFIFO_PRE_BUF                  0xDA

//0xE0 - 0xEF AES_KEY
//0xDB - 0xDF Not Used
//0xE0 - 0xFF AES Workspace
//0xF0 - 0xFF AES_BUFFER

///@}

/** @name Назначение выводов GPIO
*/                        
///@{
#define RXFIFO_THR                      0x00
#define RXFIFO_THR_PKT                  0x01
#define TXFIFO_THR                      0x02
#define TXFIFO_THR_PKT                  0x03
#define RXFIFO_OVERFLOW                 0x04 
#define TXFIFO_UNDERFLOW                0x05
#define PKT_SYNC_RXTX                   0x06
#define CRC_OK                          0x07
#define SERIAL_CLK                      0x08
#define SERIAL_RX                       0x09

//0x0A - Reserved

#define PQT_REACHED                     0x0B
#define PQT_VALID                       0x0C
#define RSSI_VALID                      0x0D

#define RSSI_UPDATE                     0x0E
#define AGC_HOLD                        0x0E
#define AGC_UPDATE                      0x0E

#define CCA_STATUS                      0x0F
#define TXONCCA_DONE                    0x0F
#define TXONCCA_FAILED                  0x0F

#define CARRIER_SENSE_VALID             0x10
#define CARRIER_SENSE                   0x11

#define DSSS_CLK                        0x12
#define DSSS_DATA0                      0x12
#define DSSS_DATA1                      0x12

#define PKT_CRC_OK                      0x13
#define MCU_WAKEUP                      0x14
#define SYNC_LOW0_HIGH1                 0x15
#define AES_COMMAND_ACTIVE              0x16
#define LNA_PA_REG_PD                   0x17
#define LNA_PD                          0x18
#define PA_PD                           0x19
#define RX0TX1_CFG                      0x1A

//0x1B - Reserved

#define IMAGE_FOUND                     0x1C
#define CLKEN_CFM                       0x1D
///@}

/** @name Значения конфигурационных регистров
*/                        
///@{

/** @name SYNC_CFG1
*/                        
///@{
#define SYNC_MODE_NO_SYNC               0x00
#define SYNC_MODE_11_BITS               0x20
#define SYNC_MODE_16_BITS               0x40
#define SYNC_MODE_18_BITS               0x60
#define SYNC_MODE_24_BITS               0x80
#define SYNC_MODE_32_BITS               0xA0
#define SYNC_MODE_16H_BITS              0xC0
#define SYNC_MODE_16D_BITS              0xE0
///@}

/** @name MODCFG_DEV_E
*/                        
///@{
#define MODEM_MODE_NORMAL               0x00
#define MODEM_MODE_DSSS_REPEAT          0x40
#define MODEM_MODE_DSSS_PN              0x80
#define MODEM_MODE_CARRIER_SENSE        0xC0

#define MOD_FORMAT_2_FSK                0x00
#define MOD_FORMAT_2_GFSK               0x08
#define MOD_FORMAT_ASK_OOK              0x18
#define MOD_FORMAT_4_FSK                0x20
#define MOD_FORMAT_4_GFSK               0x28
///@}

/** @name DCFILT_CFG
*/                        
///@{
#define DCFILT_FREEZE_COEFF_MANUAL      0x00
#define DCFILT_FREEZE_COEFF_FILTER      0x40

#define DCFILT_BW_SETTLE_8_SAMPLES      0x00
#define DCFILT_BW_SETTLE_16_SAMPLES     0x08
#define DCFILT_BW_SETTLE_32_SAMPLES     0x10
#define DCFILT_BW_SETTLE_64_SAMPLES     0x18
#define DCFILT_BW_SETTLE_128_SAMPLES    0x20
///@}

/** @name PREAMBLE_CFG0
*/                        
///@{
#define PQT_EN_DISABLED                 0x00
#define PQT_EN_ENABLED                  0x80

#define PQT_VALID_TIMEOUT_11_SYMBOLS    0x00
#define PQT_VALID_TIMEOUT_12_SYMBOLS    0x10
#define PQT_VALID_TIMEOUT_13_SYMBOLS    0x20
#define PQT_VALID_TIMEOUT_15_SYMBOLS    0x30
#define PQT_VALID_TIMEOUT_16_SYMBOLS    0x40
#define PQT_VALID_TIMEOUT_17_SYMBOLS    0x50
#define PQT_VALID_TIMEOUT_24_SYMBOLS    0x60
#define PQT_VALID_TIMEOUT_32_SYMBOLS    0x70
///@}

/** @name IQIC
*/                        
///@{
#define IQIC_EN_DISABLED                0x00
#define IQIC_EN_ENABLED                 0x80

#define IQIC_UPDATE_COEFF_EN_DISABLED   0x00
#define IQIC_UPDATE_COEFF_EN_ENABLED    0x40

#define IQIC_BLEN_SETTLE_8_SAMPLES      0x00
#define IQIC_BLEN_SETTLE_32_SAMPLES     0x10
#define IQIC_BLEN_SETTLE_128_SAMPLES    0x20
#define IQIC_BLEN_SETTLE_256_SAMPLES    0x30

#define IQIC_BLEN_8_SAMPLES             0x00
#define IQIC_BLEN_32_SAMPLES            0x04
#define IQIC_BLEN_128_SAMPLES           0x08
#define IQIC_BLEN_256_SAMPLES           0x0C

#define IQIC_IMGCH_LEVEL_THR_256        0x00
#define IQIC_IMGCH_LEVEL_THR_512        0x01
#define IQIC_IMGCH_LEVEL_THR_1024       0x02
#define IQIC_IMGCH_LEVEL_THR_2048       0x03
///@}

/** @name CHAN_BW
*/                        
///@{
#define ADC_CIC_DECFACT_12              0x00
#define ADC_CIC_DECFACT_24              0x40
#define ADC_CIC_DECFACT_48              0x80
///@}

/** @name MDMCFG2
*/                        
///@{
#define ASK_SHAPE_8                     0x00
#define ASK_SHAPE_16                    0x40
#define ASK_SHAPE_32                    0x80
#define ASK_SHAPE_128                   0xC0

#define UPSAMPLER_P_1                   0x00
#define UPSAMPLER_P_2                   0x02
#define UPSAMPLER_P_4                   0x04
#define UPSAMPLER_P_8                   0x06
#define UPSAMPLER_P_16                  0x08
#define UPSAMPLER_P_32                  0x0A
#define UPSAMPLER_P_64                  0x0C

#define CFM_DATA_EN_DISABLED            0x00
#define CFM_DATA_EN_ENABLED             0x01
///@}

/** @name MDMCFG1
*/                        
///@{
#define CARRIER_SENSE_GATE_SEARCH       0x00
#define CARRIER_SENSE_GATE_DONT_SEARCH  0x80

#define FIFO_EN_DISABLED                0x00
#define FIFO_EN_ENABLED                 0x40

#define MANCHESTER_EN_DISABLED          0x00
#define MANCHESTER_EN_ENABLED           0x20

#define INVERT_DATA_EN_DISABLED         0x00
#define INVERT_DATA_EN_ENABLED          0x10

#define COLLISION_DETECT_EN_DISABLED    0x00
#define COLLISION_DETECT_EN_ENABLED     0x08

#define DVGA_GAIN_0_DB                  0x00
#define DVGA_GAIN_MINUS_18_DB           0x02

#define SINGLE_ADC_EN_DISABLED          0x00
#define SINGLE_ADC_EN_ENABLED           0x01
///@}

/** @name MDMCFG0
*/                        
///@{
#define TRANSPARENT_MODE_EN_DISABLED    0x00
#define TRANSPARENT_MODE_EN_ENABLED     0x40

#define TRANSPARENT_INTFACT_1           0x00
#define TRANSPARENT_INTFACT_2           0x10
#define TRANSPARENT_INTFACT_4           0x20

#define DATA_FILTER_EN_DISABLED         0x00
#define DATA_FILTER_EN_ENABLED          0x08

#define VITERBI_EN_DISABLED             0x00
#define VITERBI_EN_ENABLED              0x04
///@}

/** @name AGC_CFG2
*/                        
///@{
#define START_PREVIOUS_GAIN_EN_DISABLED 0x00
#define START_PREVIOUS_GAIN_EN_ENABLED  0x80

#define FE_PERFORMANCE_MODE_OPT_LINEAR  0x00
#define FE_PERFORMANCE_MODE_NORMAL      0x20
#define FE_PERFORMANCE_MODE_LOW_POWER   0x40
#define FE_PERFORMANCE_MODE_ZERO_IF     0x60
///@}

/** @name AGC_CFG1
*/                        
///@{
#define RSSI_STEP_THR_3_OR_10_DB        0x00
#define RSSI_STEP_THR_6_OR_16_DB        0x40

#define AGC_WIN_SIZE_8_SAMPLES          0x00
#define AGC_WIN_SIZE_16_SAMPLES         0x08
#define AGC_WIN_SIZE_32_SAMPLES         0x10
#define AGC_WIN_SIZE_64_SAMPLES         0x18
#define AGC_WIN_SIZE_128_SAMPLES        0x20
#define AGC_WIN_SIZE_256_SAMPLES        0x28

#define AGC_SETTLE_WAIT_24_SAMPLES      0x00
#define AGC_SETTLE_WAIT_32_SAMPLES      0x01
#define AGC_SETTLE_WAIT_40_SAMPLES      0x02
#define AGC_SETTLE_WAIT_48_SAMPLES      0x03
#define AGC_SETTLE_WAIT_64_SAMPLES      0x04
#define AGC_SETTLE_WAIT_80_SAMPLES      0x05
#define AGC_SETTLE_WAIT_96_SAMPLES      0x06
#define AGC_SETTLE_WAIT_127_SAMPLES     0x07
///@}

/** @name AGC_CFG0
*/                        
///@{
#define AGC_HYST_LEVEL_2_DB             0x00
#define AGC_HYST_LEVEL_4_DB             0x40
#define AGC_HYST_LEVEL_7_DB             0x80
#define AGC_HYST_LEVEL_10_DB            0xC0

#define AGC_SLEWRATE_LIMIT_60_DB        0x00
#define AGC_SLEWRATE_LIMIT_30_DB        0x10
#define AGC_SLEWRATE_LIMIT_18_DB        0x20
#define AGC_SLEWRATE_LIMIT_9_DB         0x30

#define RSSI_VALID_CNT_1                0x00
#define RSSI_VALID_CNT_2                0x04
#define RSSI_VALID_CNT_5                0x08
#define RSSI_VALID_CNT_9                0x0C

#define AGC_ASK_DECAY_1200_SAMPLES      0x00
#define AGC_ASK_DECAY_2400_SAMPLES      0x01
#define AGC_ASK_DECAY_4700_SAMPLES      0x02
#define AGC_ASK_DECAY_9500_SAMPLES      0x03
///@}

/** @name FIFO_CFG
*/                        
///@{
#define CRC_AUTOFLUSH_DISABLED          0x00
#define CRC_AUTOFLUSH_ENABLED           0x80
///@}

/** @name FS_CFG
*/                        
///@{
#define FS_LOCK_EN_DISABLED             0x00
#define FS_LOCK_EN_ENABLED              0x10

#define FSD_BANDSELECT_LO_DIV_4         0x02
#define FSD_BANDSELECT_LO_DIV_8         0x04
#define FSD_BANDSELECT_LO_DIV_12        0x06
#define FSD_BANDSELECT_LO_DIV_16        0x08
#define FSD_BANDSELECT_LO_DIV_20        0x0A
#define FSD_BANDSELECT_LO_DIV_24        0x0B
///@}

/** @name PKT_CFG2
*/                        
///@{
#define BYTE_SWAP_EN_DISABLED           0x00
#define BYTE_SWAP_EN_ENABLED            0x40

#define FG_MODE_EN_DISABLED             0x00
#define FG_MODE_EN_ENABLED              0x20

#define CCA_MODE_ALWAYS                 0x00
#define CCA_MODE_RSSI                   0x04
#define CCA_MODE_CUR_RECEIVE            0x08
#define CCA_MODE_RSSI_NOT_CUR_REVEIVE   0x0C
#define CCA_MODE_RSSI_ETS_LBT           0x10

#define PKT_FORMAT_NORMAL               0x00
#define PKT_FORMAT_SYNC_SERIAL          0x01
#define PKT_FORMAT_RANDOM               0x02
#define PKT_FORMAT_TRANS_SERIAL         0x03
///@}

/** @name PKT_CFG0
*/                        
///@{
#define LENGTH_CONFIG_FIXED             0x00
#define LENGTH_CONFIG_VARIABLE_FIRST    0x20
#define LENGTH_CONFIG_INFINITE          0x40
#define LENGTH_CONFIG_VARIABLE_5LSB     0x60

#define UART_MODE_EN_DISABLED           0x00
#define UART_MODE_EN_ENABLED            0x02

#define UART_SWAP_EN_DISABLED           0x00
#define UART_SWAP_EN_ENABLED            0x01
///@}

/** @name FREQOFF_CFG
*/                        
///@{
#define FOC_EN_DISABLED                 0x00
#define FOC_EN_ENABLED                  0x20

#define FOC_CFG_AFTER_CHANNEL_FILTER    0x00
#define FOC_CFG_FS_1_128                0x08
#define FOC_CFG_FS_1_256                0x10
#define FOC_CFG_FS_1_512                0x18

#define FOC_LIMIT_BANDW_4               0x00
#define FOC_LIMIT_BANDW_8               0x04
///@}

/** @name FS_DIG0
*/                        
///@{
#define RX_LPF_BW_200_KHZ               0x00
#define RX_LPF_BW_300_KHZ               0x04
#define RX_LPF_BW_400_KHZ               0x08
#define RX_LPF_BW_500_KHZ               0x0C

#define TX_LPF_BW_200_KHZ               0x00
#define TX_LPF_BW_300_KHZ               0x01
#define TX_LPF_BW_400_KHZ               0x02
#define TX_LPF_BW_500_KHZ               0x03
///@}

/** @name FS_CAL0
*/                        
///@{
#define LOCK_CFG_AVG_512_CYCLES         0x00
#define LOCK_CFG_AVG_1024_CYCLES        0x04
#define LOCK_CFG_AVG_256_CYCLES         0x08
#define LOCK_CFG_AVG_INFINITE           0x0C
///@}

/** @name IFAMP
*/                        
///@{
#define IFAMP_BW_300_KHZ                0x00
#define IFAMP_BW_600_KHZ                0x04
#define IFAMP_BW_1000_KHZ               0x08
#define IFAMP_BW_1500_KHZ               0x0C
///@}

/** @name XOSC1
*/                        
///@{
#define XOSC_BUF_SEL_SINGLE_ENDED       0x00
#define XOSC_BUF_SEL_DIFFERENTIAL       0x02
///@}

///@}

/**
@}
*/