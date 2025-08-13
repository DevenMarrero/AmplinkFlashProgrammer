#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include "config.h"
#include "ftd2xx.h"
#include "libmpsse_i2c.h"

/*!
 * Initializes the FTDI channel for i2C
 * @param deviceNumber Index of channel to open
 * @param ftHandle Pointer to variable of type FT_Handle where handle will be stored
 * @return FT_STATUS
*/
FT_STATUS i2c_driver_init(ftd_channel_t deviceNumber, FT_HANDLE *pHandle);


/*!
 * Writes a number of bytes to a specified i2c address
 * @param[in] ftHandle Handle of the i2c channel
 * @param[in] deviceAddress Address of the I2C slave (Ambient 2 Click)
 * @param[in] data Bytes to be written
 * @param[in] numBytes Number of bytes to write
 * @return FT_STATUS
 */
FT_STATUS i2c_driver_write(FT_HANDLE ftHandle, uint8_t deviceAddress, const uint8_t *data, uint32_t numBytes);


/*!
 * Reads a number of bytes from a specified i2c address
 * @param[in] ftHandle Handle of the i2c channel
 * @param[in] deviceAddress Address of the I2C slave (Ambient 2 Click)
 * @param[in] registerAddress Address of the memory location inside the slave to start reading
 * @param[out] data Pointer to buffer to read data to
 * @param[in] numBytes Number of bytes to read
 * @return FT_STATUS
 */
FT_STATUS i2c_driver_read(FT_HANDLE ftHandle, uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, uint32_t numBytes);


/*!
 * Handles clean closing of I2C port
 * @param ftHandle Handle of port to close
 * @return FT_STATUS
*/
FT_STATUS i2c_driver_close(FT_HANDLE ftHandle);

#endif