#ifndef SPI_FLASH_h
#define SPI_FLASH_h

#include "spi_driver.h"
#include "ftd2xx.h"
#include <stdint.h>

// Sets WEL bit to 1
FT_STATUS flash_write_enable(FT_HANDLE ftHandle);

// Sets WEL bit to 0
FT_STATUS flash_write_disable(FT_HANDLE ftHandle);

// Erases flash chip and waits until operation is complete
// Returns FT_OK on successful erase
FT_STATUS flash_chip_erase(FT_HANDLE ftHandle);

FT_STATUS flash_write_page(FT_HANDLE ftHandle, uint32_t address, const uint8_t *data, uint8_t data_length);

/*!
 * @param[in] ftHandle handle of spi channel
 * @param[out] status variable to store flash status reg
 * @returns variable of type FT_STATUS
*/
FT_STATUS flash_get_status(FT_HANDLE ftHandle, uint8_t *status);

// Reads EPE bit from status register. Updated after each program/erase
int flash_success(FT_HANDLE ftHandle);

// Reads WEL bit from status register
int flash_write_isEnabled(FT_HANDLE ftHandle);

// Reads ~busy bit from status register
int flash_isReady(FT_HANDLE ftHandle);

// Reads busy bit from status register
int flash_isBusy(FT_HANDLE ftHandle);

#endif