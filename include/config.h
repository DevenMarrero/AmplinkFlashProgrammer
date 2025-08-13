#ifndef CONFIG_H
#define CONFIG_H


// amplink specific FTDI channels
typedef enum {
    I2C_CHANNEL,
    SPI_CHANNEL,
    GPIO_CHANNEL,
    CTRL_CHANNEL
} ftd_channel_t;


// config bits for controlling CS pin using libMPSSE
typedef enum {
    SPI_CS_1    = 0x00,
    SPI_CS_2    = 0x04,
    SPI_CS_3    = 0x08,
    SPI_CS_4    = 0x0C,
} spi_chip_select_t;


// GPIO channel 2
#define GPIO_AMP_CTRL    (1 << 0)
#define GPIO_AMP_ALERT   (1 << 1)
#define GPIO_FLASH_WP    (1 << 2)
#define GPIO_FLASH_RST   (1 << 3)
#define GPIO_AMP_EN      (1 << 4)
#define GPIO_AMP_CONFIG  (1 << 5)
#define GPIO_CH2_UNUSED1 (1 << 6) // NC
#define GPIO_CH2_UNUSED2 (1 << 7) // NC

// GPIO channel 3
#define GPIO_LED         (1 << 0)
#define GPIO_SPI_OEN     (1 << 1)
#define GPIO_SPI_S       (1 << 2)
#define GPIO_CH3_UNUSED1 (1 << 3) // NC
#define GPIO_CH3_UNUSED2 (1 << 4) // NC
#define GPIO_CH3_UNUSED3 (1 << 5) // NC
#define GPIO_CH3_UNUSED4 (1 << 6) // NC
#define GPIO_CH3_UNUSED5 (1 << 7) // NC


#endif