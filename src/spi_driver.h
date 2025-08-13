#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>
#include "config.h"
#include "ftd2xx.h"

/*!
 * Initializes the FTDI channel for SPI
 * @param deviceNumber Variable of type ftd_channel_t
 * @param ftHandle Pointer to variable of type FT_Handle where handle will be stored
 * @return variable of type FT_STATUS
*/
FT_STATUS spi_driver_init(ftd_channel_t deviceNumber, FT_HANDLE *pHandle);

/*!
 * Sets chipselect for ftdxx device
 * @param ftHandle Pointer to variable of type FT_Handle where handle will be stored
 * @param chipSelect Variable of type spi_chip_select_t
 * @return variable of type FT_STATUS
*/
FT_STATUS spi_driver_setCS(FT_HANDLE ftHandle, spi_chip_select_t chipSelect);

/*!
 * Writes bytes over SPI
 * @param ftHandle Pointer to variable of type FT_Handle where handle will be stored
 * @param[in] tx_buff Pointer to array of bytes to send
 * @param num_Bytes Number of bytes to write from buffer
 * @return variable of type FT_STATUS
*/
FT_STATUS spi_driver_write(FT_HANDLE ftHandle, uint8_t *tx_buff, uint32_t numBytes);

/*!
 * Writes and then reads bytes from SPI
 * @param ftHandle Pointer to variable of type FT_Handle where handle will be stored
 * @param[in] tx_buff Pointer to array of bytes to send
 * @param num_write Number of bytes to write from tx_buff
 * @param[out] rx_buff Pointer to array of where to read byes
 * @param num_read Number of bytes to read to rx_buff
 * @return variable of type FT_STATUS
*/
FT_STATUS spi_driver_transfer(FT_HANDLE ftHandle, uint8_t *tx_buff, uint32_t num_write, uint8_t *rx_buff, uint32_t num_read);

/*!
 * Handles clean closing of SPI port
 * @param ftHandle Handle of port to close
 * @return variable of type FT_STATUS
*/
FT_STATUS spi_driver_close(FT_HANDLE ftHandle);

#endif