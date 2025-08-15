/*! @file programmer.h
    @brief Handles AmpLink to Processor board specific protocols
    
    Provides the main interface for programming the Processor board through the AmpLink.
    Provides functions for all needed flash/clock commands and abstracts AmpLink away from the user
    Also contains some passthroughs for direct SPI/I2C communication
*/

#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <windows.h>
#include <stdint.h>

#include "config.h"
#include "ftd2xx.h"

//! Opens ftdi GPIO, SPI, and I2C ports
FT_STATUS programmer_init(void);

/*!
 * @brief Selects processor board flash chip mux and sets SPI_driver CS
 *
 * @param chipSelect variable of type spi_chip_select_t
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_flash_select_chip(spi_chip_select_t chipSelect);

/*!
 * @brief Writes data to flash memory over SPI
 *
 * Data is automatically chunked into 256 byte pages with seperate writes
 * 
 * @param address address of flash memory to start writing
 * @param data pointer to array of type uint8_t to program
 * @param length number of bytes in data
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_flash_write_page(uint32_t address, const uint8_t *data, uint8_t length);

FT_STATUS programmer_flash_verify_page(uint32_t address, const uint8_t *data, uint8_t length);

/*!
 * @brief Erases entire flash chip, takes ~500ms
 *
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_flash_erase_chip(void);

/*!
 * @brief Sets write enable bit in flash memory status register
 * 
 * @param[in] is_enabled 1=ENABLE, 0=DISABLE
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_flash_set_write_state(uint8_t is_enabled);

/*!
 * @brief Sets versaClock i2c address.
 *
 * @param[in] address i2c address of clock
 * @return FT_STATUS Status of the operation: FT_OK, FT_INVALID_PARAMETER
*/
FT_STATUS programmer_clock_set_addr(uint8_t address);

/*!
 * @brief Writes data to clock memory over i2c.
 *
 * data is automatically chunked into 256 byte pages with seperate writes
 * 
 * @param address address of flash memory to start writing
 * @param data pointer to array of type uint8_t to program
 * @param length number of bytes in data
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_clock_write_page(uint32_t address, const uint8_t *data, uint8_t length);

/*! 
 * @brief Performs versaClock burn sequence
 *
 * @return FT_STATUS Status of the operation
 */
FT_STATUS programmer_clock_burn(void);

FT_STATUS programmer_clock_verify_page(uint32_t address, const uint8_t *data, uint8_t length);

/*!
 * @brief Writes bytes directly out to SPI channel
 *
 * @param[in] tx_buff array of type uint8_t of bytes to write 
 * @param[in] numBytes number of bytes to send
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_spi_write(uint8_t *tx_buff, uint32_t numBytes);

/*!
 * @brief Performs a write and immediate read over SPI
 *
 * @param[in] tx_buff array of type uint8_t of bytes to write 
 * @param[in] num_write number of bytes to send
 * @param[out] rx_buff array of type uint8_t where bytes will be read to
 * @param[in] num_read number of bytes to read
 * @return FT_STATUS Status of the operation
*/
FT_STATUS programmer_spi_transfer(uint8_t *tx_buff, uint32_t num_write, uint8_t *rx_buff, uint32_t num_read);

/*! 
 * @brief Close all related ports on the AmPLink device.
 */
void programmer_close(void);

#endif