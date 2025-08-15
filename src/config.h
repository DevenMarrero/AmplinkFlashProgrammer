/*!
 * @file config.h
 * @brief Configuration definitions for AmPLink FTDI channels and GPIO mapping.
 *
 * This header file provides enumerations and macro definitions to configure
 * the FT4232H within the AmPLink device. It includes:
 * - Channel identifiers for FTDI interfaces
 * - SPI chip select bitmasks for libMPSSE usage
 * - GPIO bitmask definitions for FTDI channels 2 & 3
 *
 * @details
 * These definitions are used throughout the AmPLink drivers.
 * Use these constants within the functions defined in:
 * - @ref spi_driver.h
 * - @ref gpio_driver.h
 *
 * @note Ensure the correct FTDI channels are initialized before using macros or enums.
 *
 * @date 2025-08-14
 * @author Deven Marrero
*/

#ifndef CONFIG_H
#define CONFIG_H


/*!
 * @enum ftd_channel_t
 * @brief Identifies the specific FTDI channels used by the AmPLink device
 *
 * This enum defines the channels available on the ft4232H and how they
 * are utilized by the AmPLink controller.
 */
typedef enum {
    I2C_CHANNEL,  /*!< I2C communication channel */
    SPI_CHANNEL,  /*!< SPI communication channel */
    GPIO_CHANNEL, /*!< AmPLink connector GPIO channel */
    CTRL_CHANNEL  /*!< AmPLink internal GPIO channel */
} ftd_channel_t;

/*!
 * @enum spi_chip_select_t
 * @brief Specifies the available SPI chip select lines.
 *
 * Sets config bits for selecting CS pin using libMPSSE
*/
typedef enum {
    SPI_CS_1    = 0x00, /*!< Chip select line 1 */
    SPI_CS_2    = 0x04, /*!< Chip select line 2 */
    SPI_CS_3    = 0x08, /*!< Chip select line 3 */
    SPI_CS_4    = 0x0C, /*!< Chip select line 4 */
} spi_chip_select_t;

/*!
 * @name GPIO Channel 2 Definitions
 * @brief Bitmask values for GPIO pins on ftd channel 2. See @ref ftd_channel_t.
 *
 * These macros represent the bit postions for GPIO pins on
 * channel 2. Used to read and write pins on the AmPLink connector. 
 * See @ref gpio_driver.h for functions.
 * @{
 */
#define GPIO_AMP_CTRL    (1 << 0) /*!< AMP_CTRL pin */
#define GPIO_AMP_ALERT   (1 << 1) /*!< AMP_ALERT pin */
#define GPIO_FLASH_WP    (1 << 2) /*!< FLASH_WP pin */
#define GPIO_FLASH_RST   (1 << 3) /*!< FLASH_RST pin */
#define GPIO_AMP_EN      (1 << 4) /*!< AMP_EN pin */
#define GPIO_AMP_CONFIG  (1 << 5) /*!< AMP_CONFIG pin */
#define GPIO_CH2_UNUSED1 (1 << 6) /*!< Not connected (NC) */
#define GPIO_CH2_UNUSED2 (1 << 7) /*!< Not connected (NC) */
/*! @} */

/*!
 * @name GPIO Channel 3 Definitions
 * @brief Bitmask values for GPIO pins on ftd channel 3. See @ref ftd_channel_t.
 *
 * These macros represent the bit postions for GPIO pins on
 * channel 3. Used to read and write pins internally on the AmPLink. 
 * See @ref gpio_driver.h for functions.
 * @{
 */
#define GPIO_LED         (1 << 0) /*!< AmPLink external LED */
#define GPIO_SPI_OEN     (1 << 1) /*!< Mux ouput enable for SPI channel signals (set 0) */
#define GPIO_SPI_S       (1 << 2) /*!< Mux select for SPI channel signals (set 1) */
#define GPIO_CH3_UNUSED1 (1 << 3) /*!< Not connected (NC) */
#define GPIO_CH3_UNUSED2 (1 << 4) /*!< Not connected (NC) */
#define GPIO_CH3_UNUSED3 (1 << 5) /*!< Not connected (NC) */
#define GPIO_CH3_UNUSED4 (1 << 6) /*!< Not connected (NC) */
#define GPIO_CH3_UNUSED5 (1 << 7) /*!< Not connected (NC) */
/*! @} */

#endif