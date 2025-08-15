#include "programmer.h"

#include "utils.h"
#include "spi_flash.h"
#include "gpio_driver.h"
#include "spi_driver.h"
#include "i2c_driver.h"

#define AMPLINK_CHANNEL_NUM 4 // amplink programmer will always have 4 channels

/*!
 * @struct ProgrammerContext
 * @brief Context holding all handles for a single FTDI device.
 * 
 * Stores the handles for the 4 FTDI communication channels present on the FT4232.
 * 
 * @note These handles must be passed to the corresponding driver file
 * @see spi_driver.h, gpio_driver.h, i2c_driver.h
 * 
 */
typedef struct {
    FT_HANDLE ftSPIHandle;  /*!< Handle for SPI communication channel */
    FT_HANDLE ftI2CHandle;  /*!< Handle for I2C communication channel */
    FT_HANDLE ftGPIOHandle; /*!< Handle for general GPIO channel */
    FT_HANDLE ftCTRLHandle; /*!< Handle for internal control GPIO channel */
} ProgrammerContext;

//! Global static instance of ProgrammerContext
static ProgrammerContext device;
//! Static variable of the versaClock i2c address
static uint8_t i2c_addr;


FT_STATUS programmer_init(void){
    FT_STATUS ftStatus;
    DWORD numDevs;

    // amplink has 4 channels, use this to detect device
    RETURN_IF_ERROR(FT_CreateDeviceInfoList(&numDevs));
    if (numDevs != AMPLINK_CHANNEL_NUM) 
        return FT_OTHER_ERROR;
    // open GPIO ports
    RETURN_IF_ERROR(gpio_driver_init(GPIO_CHANNEL, &device.ftGPIOHandle));
    RETURN_IF_ERROR(gpio_driver_init(CTRL_CHANNEL, &device.ftCTRLHandle));
    RETURN_IF_ERROR(gpio_driver_write_pin(device.ftCTRLHandle, GPIO_LED, 1)); // set LED
    // SPI
    RETURN_IF_ERROR(spi_driver_init(SPI_CHANNEL, &device.ftSPIHandle));
    RETURN_IF_ERROR(gpio_driver_write_pin(device.ftCTRLHandle, GPIO_SPI_OEN, 0)); // set internal spi line mux
    RETURN_IF_ERROR(gpio_driver_write_pin(device.ftCTRLHandle, GPIO_SPI_S, 1));
    // I2C
    RETURN_IF_ERROR(i2c_driver_init(I2C_CHANNEL, &device.ftI2CHandle));
    return FT_OK;
}

FT_STATUS programmer_flash_select_chip(spi_chip_select_t chipSelect){
    unsigned char mode_pin;
    unsigned char en_pin;
    // set onebox processor mux
    switch (chipSelect){
        case SPI_CS_2:
            mode_pin = GPIO_AMP_CTRL;
            en_pin = GPIO_FLASH_WP;
            break;
        case SPI_CS_3:
            mode_pin = GPIO_AMP_CONFIG;
            en_pin = GPIO_AMP_ALERT;
            break;
        case SPI_CS_4:
            mode_pin = GPIO_FLASH_RST;
            en_pin = GPIO_AMP_EN;
            break;
        default:
            return FT_OTHER_ERROR;
    }
    RETURN_IF_ERROR(gpio_driver_write_pin(device.ftGPIOHandle, mode_pin, 1));
    RETURN_IF_ERROR(gpio_driver_write_pin(device.ftGPIOHandle, en_pin, 0));
    return spi_driver_setCS(device.ftSPIHandle, chipSelect);
}

FT_STATUS programmer_flash_write_page(uint32_t address, const uint8_t *data, uint8_t length){
    FT_STATUS ftStatus;

    while (length > 0) {
        // calculate space left on current flash page
        uint32_t page_offset = address % 256;
        uint32_t space_left = 256 - page_offset;

        // limit write length to smaller of data or page space
        uint8_t chunk_length = (length <= space_left) ? length : space_left;

        //printf("Writing %d bytes to address 0x%06X\n", chunk_length, address);
        ftStatus = flash_write_page(device.ftSPIHandle, address, data, chunk_length);

        // move to next chunk
        address += chunk_length;
        data += chunk_length;
        length -= chunk_length;
    }
    return ftStatus;
}

FT_STATUS programmer_flash_verify_page(uint32_t address, const uint8_t *data, uint8_t length){

}

FT_STATUS programmer_flash_erase_chip(void){
    return flash_chip_erase(device.ftSPIHandle);
}

FT_STATUS programmer_flash_set_write_state(uint8_t enable){
    if (enable)
        return flash_write_enable(device.ftSPIHandle);
    else
        return flash_write_disable(device.ftSPIHandle);
}

FT_STATUS programmer_clock_set_addr(uint8_t address){
    if (address >= 0x00 && address <= 0xFF){
        i2c_addr = address;
        return FT_OK;
    }
    return FT_INVALID_PARAMETER;
}

FT_STATUS programmer_clock_write_page(uint32_t address, const uint8_t *data, uint8_t length){
    FT_STATUS ftStatus;
    if (!i2c_addr){
        return FT_INVALID_PARAMETER;
    }

    while (length > 0) {
        // calculate space left on current flash page
        uint32_t page_offset = address % 256;
        uint32_t space_left = 256 - page_offset;

        // limit write length to smaller of data or page space
        uint8_t chunk_length = (length <= space_left) ? length : space_left;

        //printf("Writing %d bytes to address 0x%04X\n", chunk_length, address);
        // address
        uint8_t buffer[256];
        buffer[0] = (uint8_t)(address >> 8);
        buffer[1] = (uint8_t)(address);

        // data
        memcpy(buffer + 2, data, chunk_length);
        ftStatus = (i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, chunk_length + 2));

        // move to next chunk
        address += chunk_length;
        data += chunk_length;
        length -= chunk_length;
    }
    return ftStatus;
}

FT_STATUS programmer_clock_burn(void){
    uint8_t buffer[3];
    // OTP burn
    buffer[0] = 0x00;
    buffer[1] = 0x72;
    buffer[2] = 0xF0;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));
    buffer[2] = 0xF8;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));
    Sleep(500);
    buffer[2] = 0xF0;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));
    buffer[2] = 0xF8;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));
    Sleep(500);
    buffer[2] = 0xF0;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));
    buffer[2] = 0xF2;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));
    buffer[2] = 0xF0;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 3));

    // read 0x9F, if D1=0 success
    uint8_t status_reg;
    RETURN_IF_ERROR(i2c_driver_read(device.ftI2CHandle, i2c_addr, 0x9F, &status_reg, 1));

    if ((status_reg & 0x02) == 0x02){
        return FT_FAILED_TO_WRITE_DEVICE;
    }  

    // clear status reg
    buffer[0] = 0x9F;
    buffer[1] = 0x00;
    RETURN_IF_ERROR(i2c_driver_write(device.ftI2CHandle, i2c_addr, buffer, 2));
    return FT_OK;
}

FT_STATUS programmer_clock_verify_page(uint32_t address, const uint8_t *data, uint8_t length){

}

FT_STATUS programmer_spi_write(uint8_t *tx_buff, uint32_t numBytes){
    return spi_driver_write(device.ftSPIHandle, tx_buff, numBytes);
}

FT_STATUS programmer_spi_transfer(uint8_t *tx_buff, uint32_t num_write, uint8_t *rx_buff, uint32_t num_read){
    return spi_driver_transfer(device.ftSPIHandle, tx_buff, num_write, rx_buff, num_read);
}


void programmer_close(void){
    gpio_driver_close(device.ftGPIOHandle);
    gpio_driver_close(device.ftCTRLHandle);
    spi_driver_close(device.ftSPIHandle);
    i2c_driver_close(device.ftI2CHandle);
}

