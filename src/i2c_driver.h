/*! @file i2c_driver.h
 *  @brief I2C driver for interfacing with ftd2xx devices.
 *
 * Provides functions for initializing, writing to, reading from,
 * and closing I2C channels of FTDI devices
 * 
 * @details
 * All functions return an'FT_STATUS' value from the FTD2xx library.
 * Handles of type 'FT_HANDLE' are used to reference the open I2C channel.
 * 
 * @note Ensure the correct channel is initialized and passed to the write/read functions.
 * 
 * @date 2025-08-14
 * @author Deven Marrero
*/

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include "config.h"
#include "ftd2xx.h"
#include "libmpsse_i2c.h"

/*!
 * @brief Initializes the FTDI channel for i2C
 *
 * @param[in] deviceNumber Index of channel to open
 * @param[out] pHandle Pointer to variable of type FT_Handle where handle will be stored
 * @return FT_STATUS
*/
FT_STATUS i2c_driver_init(ftd_channel_t deviceNumber, FT_HANDLE *pHandle);


/*!
 * @brief Writes a number of bytes to a specified i2c address
 *
 * @param[in] ftHandle Handle of the i2c channel
 * @param[in] deviceAddress Address of the I2C slave (Ambient 2 Click)
 * @param[in] data Bytes to be written
 * @param[in] numBytes Number of bytes to write
 * @return FT_STATUS
 */
FT_STATUS i2c_driver_write(FT_HANDLE ftHandle, uint8_t deviceAddress, uint8_t *data, uint32_t numBytes);


/*!
 * @brief Reads a number of bytes from a specified i2c address
 * 
 * @param[in] ftHandle Handle of the i2c channel
 * @param[in] deviceAddress Address of the I2C slave (Ambient 2 Click)
 * @param[in] registerAddress Address of the memory location inside the slave to start reading
 * @param[out] data Pointer to buffer to read data to
 * @param[in] numBytes Number of bytes to read
 * @return FT_STATUS
 */
FT_STATUS i2c_driver_read(FT_HANDLE ftHandle, uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, uint32_t numBytes);


/*!
 * @brief Handles clean closing of I2C port
 *
 * @param[in] ftHandle Handle of port to close
 * @return FT_STATUS
*/
FT_STATUS i2c_driver_close(FT_HANDLE ftHandle);

#endif