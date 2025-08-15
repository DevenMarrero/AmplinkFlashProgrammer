/**
 * @file spi_flash.h
 * @brief Functions for interfacing with AT25DF512C SPI Serial Flash Memory chips.
 *
 * Implements functions for controlling and accessing AT25DF512C SPI flash chips via FTDI SPI channels.
 * This includes write enable/disable, chip erase, page write, and status register operations.
 *
 * @details
 * All functions return an FT_STATUS value where applicable. Status-related functions
 * return integer flags (0 or 1). The SPI handle (`FT_HANDLE`) must
 * be properly initialized before calling any of these functions.
 *
 * @author Deven Marrero
 * @date 2025-08-14
 */


#ifndef SPI_FLASH_h
#define SPI_FLASH_h

#include "spi_driver.h"
#include "ftd2xx.h"
#include <stdint.h>

/*!
 * @brief Sets the WEL (Write Enable Latch) bit to 1.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return FT_STATUS Status of the operation
*/
FT_STATUS flash_write_enable(FT_HANDLE ftHandle);

/*!
 * @brief Sets the WEL (Write Enable Latch) bit to 0.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return FT_STATUS Status of the operation
*/
FT_STATUS flash_write_disable(FT_HANDLE ftHandle);

/*!
 * @brief Erases flash chip and waits until operation is complete.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return FT_STATUS Status of the operation
*/
FT_STATUS flash_chip_erase(FT_HANDLE ftHandle);

/**
 * @brief Writes a page of data to the flash memory.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @param[in] address Address in flash memory to write to
 * @param[in] data Pointer to the data buffer to write
 * @param[in] data_length Number of bytes to write (max page size)
 * @return FT_STATUS Status of the operation
 */
FT_STATUS flash_write_page(FT_HANDLE ftHandle, uint32_t address, const uint8_t *data, uint8_t data_length);

/*!
 * @brief Reads the flash status register.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @param[out] status Pointer to store flash status register
 * @returns FT_STATUS Status of the operation
*/
FT_STATUS flash_get_status(FT_HANDLE ftHandle, uint8_t *status);

/*!
 * @brief Reads EPE (Erase/Program Error) bit from the flash status register.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return int Returns 1 if last program/erase succeeded, o therwise
*/
int flash_success(FT_HANDLE ftHandle);

/**
 * @brief Reads the WEL (Write Enable Latch) bit from the flash status register.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return int Returns 1 if write is enabled, 0 otherwise
 */
int flash_write_isEnabled(FT_HANDLE ftHandle);

/**
 * @brief Reads the ~BUSY bit from the flash status register.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return int Returns 1 if flash is ready, 0 if busy
 */
int flash_isReady(FT_HANDLE ftHandle);

/**
 * @brief Reads the BUSY bit from the flash status register.
 *
 * @param[in] ftHandle Handle of the SPI channel
 * @return int Returns 1 if flash is busy, 0 if ready
 */
int flash_isBusy(FT_HANDLE ftHandle);

#endif