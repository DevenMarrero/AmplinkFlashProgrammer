#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "ftd2xx.h"
#include "config.h"


/*!
 * Initializes the FTDI device for GPIO bit-bang mode
 * This should be called after the FTDI GPIO channel is opened
 * @param deviceNumber Index of channel to open
 * @param ftHandle Pointer to variable of type FT_Handle where handle will be stored
 * @return FT_STATUS
*/
FT_STATUS gpio_driver_init(ftd_channel_t deviceChannel, FT_HANDLE *pHandle);

/*!
 * Reads the current logic level of a given GPIO pin
 * @param[in] ftHandle Handle of GPIO port to read
 * @param[out] value Pointer to variable to store status
 * @return status of port
*/
FT_STATUS gpio_driver_read_port(FT_HANDLE ftHandle, unsigned char *value);

/*!
 * Reads the current logic level of a given GPIO pin
 * @param ftHandle Handle of GPIO port to read
 * @param pin Pin to read
 * @return HIGH (1) if pin is on LOW (0) if pin is low
*/
uint8_t gpio_driver_read_pin(FT_HANDLE ftHandle, uint8_t pin);

/*!
 * Writes a byte to the entire port
 * @param ftHandle Handle of GPIO port to write
 * @param value Data to write to port
 * @return FT_STATUS
*/
FT_STATUS gpio_driver_write_port(FT_HANDLE ftHandle, unsigned char value);

/*!
 * Set a specific GPIO pin high or low
 * @param ftHandle Handle of GPIO port to read
 * @param pin Pin to set value of
 * @param value 1 = High, 0 = low
 * @return byte status of entire port
*/
FT_STATUS gpio_driver_write_pin(FT_HANDLE ftHandle, uint8_t pin, uint8_t value);

/*!
 * Sets pin direction of 
 * @param ftHandle Handle of port to set
 * @param pin Pin to set the direction of
 * @param dir input = 0, output = 1
 * @return FT_STATUS
*/
FT_STATUS gpio_driver_set_pin_direction(FT_HANDLE ftHandle, uint8_t pin, uint8_t dir);

/*!
 * Sets individual input/output of entire port
 * @param ftHandle Handle of port to set
 * @param dir Byte where each bit is pin direction: input = 0, output = 1
 * @return FT_STATUS
*/
FT_STATUS gpio_driver_set_port_direction(FT_HANDLE ftHandle, uint8_t dir);

/*!
 * Handles clean closing of GPIO port
 * @param ftHandle Handle of port to close
 * @return FT_STATUS
*/
FT_STATUS gpio_driver_close(FT_HANDLE ftHandle);

#endif