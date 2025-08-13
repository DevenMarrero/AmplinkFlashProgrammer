#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <windows.h>
#include <stdint.h>

#include "config.h"
#include "ftd2xx.h"

// open GPIO, SPI, & I2C ports
FT_STATUS programmer_init(void);

/*!
 * Selects processor board flash chip mux and sets SPI_driver CS
 * @param chipSelect variable of type spi_chip_select_t
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_flash_select_chip(spi_chip_select_t chipSelect);

/*!
 * Writes data to flash memory over SPI
 * data is automatically chunked into 256 byte pages with seperate writes
 * @param address address of flash memory to start writing
 * @param data pointer to array of type uint8_t to program
 * @param length number of bytes in data
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_flash_write_page(uint32_t address, const uint8_t *data, uint8_t length);

FT_STATUS programmer_flash_verify_page(uint32_t address, const uint8_t *data, uint8_t length);

/*!
 * Erases entire flash chip
 * takes ~500ms
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_flash_erase_chip(void);

/*!
 * Sets write enable bit in flash memory status register
 * @param is_enabled 1-Enable, 0-Disable
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_flash_set_write_state(uint8_t is_enabled);

/*!
 * Sets versaClock i2c address
 * @param address i2c address of clock
 * @return variable of type FT_STATUS: FT_OK, FT_INVALID_PARAMETER
*/
FT_STATUS programmer_clock_set_addr(uint8_t address);

/*!
 * Writes data to clock memory over i2c
 * data is automatically chunked into 256 byte pages with seperate writes
 * @param address address of flash memory to start writing
 * @param data pointer to array of type uint8_t to program
 * @param length number of bytes in data
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_clock_write_page(uint32_t address, const uint8_t *data, uint8_t length);

// performs versaclock burn sequence 
FT_STATUS programmer_clock_burn(void);

FT_STATUS programmer_clock_verify_page(uint32_t address, const uint8_t *data, uint8_t length);

/*!
 * Writes bytes directly out to SPI channel
 * @param[out] tx_buff array of type uint8_t of bytes to write 
 * @param numBytes number of bytes to send
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_spi_write(uint8_t *tx_buff, uint32_t numBytes);

/*!
 * Performs a write and immediate read over SPI
 * @param[out] tx_buff array of type uint8_t of bytes to write 
 * @param num_write number of bytes to send
 * @param[in] rx_buff array of type uint8_t where bytes will be read to
 * @param num_read number of bytes to read
 * @return variable of type FT_STATUS
*/
FT_STATUS programmer_spi_transfer(uint8_t *tx_buff, uint32_t num_write, uint8_t *rx_buff, uint32_t num_read);

// close all related ports
void programmer_close(void);

#endif