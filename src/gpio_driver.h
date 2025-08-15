/*!
 * @file gpio_Driver.h
 * @brief GPIO driver for interfacing with ftd2xx devices.
 *
 * Provides functions for initializing, writing to, reading from,
 * and closing GPIO channels of FTDI devices
 * 
 * @details
 * All functions return an'FT_STATUS' value from the FTD2xx library.
 * Handles of type 'FT_HANDLE' are used to reference the open GPIO channel.
 * 
 * @note Ensure the correct channel is initialized and passed to the write/read functions.
 * 
 * @date 2025-08-14
 * @author Deven Marrero
*/


#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "ftd2xx.h"
#include "config.h"


/*!
 * @brief Initializes the FTDI device for GPIO bit-bang mode.
 *
 * @note This should be called after the FTDI GPIO channel is opened
 * 
 * @param[in] deviceChannel Index of channel to open
 * @param[out] pHandle Pointer to variable of type FT_Handle where handle will be stored
 * @return FT_STATUS Status of the Operation
*/
FT_STATUS gpio_driver_init(ftd_channel_t deviceChannel, FT_HANDLE *pHandle);

/*!
 * @brief Reads the current logic level of an entire GPIO port.
 *
 * @param[in] ftHandle Handle of GPIO port to read
 * @param[out] value Pointer to variable to store port status
 * @return FT_STATUS Status of the Operation
*/
FT_STATUS gpio_driver_read_port(FT_HANDLE ftHandle, unsigned char *value);

/*!
 * @brief Reads the current logic level of a single GPIO pin.
 *
 * @param[in] ftHandle Handle of GPIO port to read
 * @param[in] pin Pin to read. See @ref config.h
 * @return uint8 Returns 1 if the pin is HIGH, 0 if LOW
*/
uint8_t gpio_driver_read_pin(FT_HANDLE ftHandle, uint8_t pin);

/*!
 * @brief Writes a byte to the entire GPIO port.
 *
 * @param[in] ftHandle Handle of GPIO port to write
 * @param[in] value Byte value to write to port
 * @return FT_STATUS Status of the operation
*/
FT_STATUS gpio_driver_write_port(FT_HANDLE ftHandle, unsigned char value);

/*!
 * @brief Set a specific GPIO pin HIGH or LOW

 * @param[in] ftHandle Handle of GPIO port to read
 * @param[in] pin Pin to write. See @ref config.h
 * @param[in] value 1 = HIGH, 0 = LOW
 * @return FT_STATUS Status of the operation
*/
FT_STATUS gpio_driver_write_pin(FT_HANDLE ftHandle, uint8_t pin, uint8_t value);

/*!
 * @brief Sets direction of a single pin
 *
 * @param[in] ftHandle Handle of port to set
 * @param[in] pin Pin to configure. See @ref config.h
 * @param[in] dir Direction: 1 = OUTPUT, 0 = INPUT
 * @return FT_STATUS Status of the operation
*/
FT_STATUS gpio_driver_set_pin_direction(FT_HANDLE ftHandle, uint8_t pin, uint8_t dir);

/*!
 * @brief Sets direction of all pins in the GPIO port
 * @param[in] ftHandle Handle of GPIO port
 * @param[in] dir Byte representing direction of each pin: 1 = OUTPUT, 0 = INPUT
 * @return FT_STATUS Status of the operation
*/
FT_STATUS gpio_driver_set_port_direction(FT_HANDLE ftHandle, uint8_t dir);

/*!
 * @brief Handles clean closing of GPIO port.
 *
 * @param[in] ftHandle Handle of port to close
 * @return FT_STATUS Status of the operation
*/
FT_STATUS gpio_driver_close(FT_HANDLE ftHandle);

#endif