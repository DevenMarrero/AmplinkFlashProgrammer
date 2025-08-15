/*! @file spi_driver.h
 *  @brief SPI driver for interfacing with ftd2xx devices.
 *
 * Provides functions for initializing, writing to, reading from,
 * and closing SPI channels of FTDI devices
 * 
 * @details
 * All functions return an'FT_STATUS' value from the FTD2xx library.
 * Handles of type 'FT_HANDLE' are used to reference the open SPI channel.
 * 
 * @note Ensure the correct channel is initialized and passed to the write/read functions.
 * 
 * @date 2025-08-14
 * @author Deven Marrero
*/

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>
#include "config.h"
#include "ftd2xx.h"

/*!
 * @brief Initializes the selected FTDI channel for SPI
 *
 * @param[in] deviceNumber Variable of type ftd_channel_t
 * @param[in] pHandle Pointer to variable of type FT_Handle where handle will be stored
 * @return FT_STATUSFT_STATUS Status of the operation
*/
FT_STATUS spi_driver_init(ftd_channel_t deviceNumber, FT_HANDLE *pHandle);

/*!
 * @brief Sets chipselect for ftdxx device
 *
 * @param[in] ftHandle Handle of the SPI channel.
 * @param[in] chipSelect Chip select setting. see @ref spi_chip_select_t for available options.
 * @return FT_STATUS Status of the operation
*/
FT_STATUS spi_driver_setCS(FT_HANDLE ftHandle, spi_chip_select_t chipSelect);

/*!
 * @brief Writes bytes over SPI
 *
 * @param[in] ftHandle Handle of the SPI channel.
 * @param[in] tx_buff Array of bytes to send.
 * @param[in] numBytes Number of bytes to write.
 * @return FT_STATUS Status of the operation
*/
FT_STATUS spi_driver_write(FT_HANDLE ftHandle, uint8_t *tx_buff, uint32_t numBytes);

/*!
 * @brief Writes and then reads bytes from SPI
 *
 * @param[in] ftHandle Handle of the SPI channel.
 * @param[in] tx_buff Array of bytes to send.
 * @param[in] num_write Number of bytes to write.
 * @param[out] rx_buff Array to read bytes to.
 * @param[in] num_read Number of bytes to read.
 * @return FT_STATUS Status of the operation
*/
FT_STATUS spi_driver_transfer(FT_HANDLE ftHandle, uint8_t *tx_buff, uint32_t num_write, uint8_t *rx_buff, uint32_t num_read);

/*!
 * @brief Handles clean closing of SPI port
 *
 * @param[in] ftHandle Handle of the SPI channel.
 * @return FT_STATUS Status of the operation
*/
FT_STATUS spi_driver_close(FT_HANDLE ftHandle);

#endif