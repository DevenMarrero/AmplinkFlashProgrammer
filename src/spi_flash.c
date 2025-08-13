#include <stdio.h>
#include "spi_flash.h"
#include "utils.h"

#define FLASH_MAX_BUFF_LEN  256
#define FLASH_OP_LEN        1
#define FLASH_ADDR_LEN      3

#define FLASH_OP_PAGE_WRITE     0x02
#define FLASH_OP_CHIP_ERASE     0x60
#define FLASH_OP_WRITE_EN       0x06
#define FLASH_OP_WRITE_DI       0x04
#define FLASH_OP_READ_STATUS    0x05

#define FLASH_STATUS_BUSY       0x01
#define FLASH_STATUS_WE         0x02
#define FLASH_STATUS_SUCCESS    0x40


FT_STATUS flash_write_enable(FT_HANDLE ftHandle){
    FT_STATUS ftStatus;
    uint8_t buffer = FLASH_OP_WRITE_EN;
    RETURN_IF_ERROR(spi_driver_write(ftHandle, &buffer, 1));
    // verify enable is set
    uint8_t status_reg;
    ftStatus = flash_get_status(ftHandle, &status_reg);
    if (ftStatus != FT_OK) return ftStatus;
    if ((status_reg & FLASH_STATUS_WE) != 0) 
        return FT_OK;
    return FT_OTHER_ERROR;
}

FT_STATUS flash_write_disable(FT_HANDLE ftHandle){
    FT_STATUS ftStatus;
    uint8_t buffer = FLASH_OP_WRITE_DI;
    RETURN_IF_ERROR(spi_driver_write(ftHandle, &buffer, 1));
    uint8_t status_reg;
    ftStatus = flash_get_status(ftHandle, &status_reg);
    if (ftStatus != FT_OK) return ftStatus;
    if ((status_reg & FLASH_STATUS_WE) == 0) 
        return FT_OK;
    return FT_OTHER_ERROR;
}

FT_STATUS flash_chip_erase(FT_HANDLE ftHandle){
    uint8_t buffer = FLASH_OP_CHIP_ERASE;
    FT_STATUS ftStatus = spi_driver_write(ftHandle, &buffer, 1);
    if (ftStatus != FT_OK) return FT_EEPROM_ERASE_FAILED;
    
    uint8_t status_reg;
    do {
        ftStatus = flash_get_status(ftHandle, &status_reg);
        if (ftStatus != FT_OK) return FT_EEPROM_ERASE_FAILED;
        printf("\rErasing flash...  flash busy");
    } while((status_reg & FLASH_STATUS_BUSY == 1));
    
    if (flash_success(ftHandle)){
        printf("\rErasing flash...  ");
        return FT_OK;
    }
    return FT_EEPROM_ERASE_FAILED;
}

FT_STATUS flash_write_page(FT_HANDLE ftHandle, uint32_t address, const uint8_t *data, uint8_t data_length){
    FT_STATUS ftStatus;
    size_t length = FLASH_OP_LEN + FLASH_ADDR_LEN + data_length; // 2=opcode, 6=address, Max255=data_legnth
    // buffer = opcode + address + data chunk
    uint8_t buffer[FLASH_MAX_BUFF_LEN];
    buffer[0] = FLASH_OP_PAGE_WRITE;
    // Address
    for (int i = 0; i < FLASH_ADDR_LEN; i++){
        buffer[FLASH_OP_LEN + i] = (uint8_t)(address >> (16-8*i));
    }
    // data
    memcpy(buffer + FLASH_OP_LEN + FLASH_ADDR_LEN, data, data_length);
    
    // remove to 
    RETURN_IF_ERROR(spi_driver_write(ftHandle, buffer, length));
    uint8_t status_reg;
    do {
        ftStatus = flash_get_status(ftHandle, &status_reg);
    } while((status_reg & 0x01) == 1); // isBusy

    if ((status_reg & 0x40) == 0) // success
        return FT_OK;
    else {
        printf("Flash chip EPE bit set: Error while programming page\n");
        return FT_EEPROM_WRITE_FAILED;
    }
}

FT_STATUS flash_get_status(FT_HANDLE ftHandle, uint8_t *status){
    FT_STATUS ftStatus;
    uint8_t tx_buff = FLASH_OP_READ_STATUS;
    uint8_t rx_buff[2] = {0,0};
    ftStatus = spi_driver_transfer(ftHandle, &tx_buff, 1, rx_buff, 2);
    *status = rx_buff[0];
    if (*status == 0xff) return FT_EEPROM_NOT_PRESENT;
    return ftStatus;
}

int flash_success(FT_HANDLE ftHandle){
    uint8_t status;
    flash_get_status(ftHandle, &status);
    return (status & FLASH_STATUS_SUCCESS) == 0;
}

int flash_write_isEnabled(FT_HANDLE ftHandle){
    uint8_t status;
    flash_get_status(ftHandle, &status);
    printf("WE STATUS: 0x%X\n",status);
    printf("result: %d\n", status & FLASH_STATUS_WE);
    return ((status & FLASH_STATUS_WE) == FLASH_STATUS_WE);
}

int flash_isBusy(FT_HANDLE ftHandle){
    uint8_t status;
    flash_get_status(ftHandle, &status);
    return ((status & FLASH_STATUS_BUSY) == FLASH_STATUS_BUSY);
}

int flash_isReady(FT_HANDLE ftHandle){
    uint8_t status;
    flash_get_status(ftHandle, &status);
    return ((status & FLASH_STATUS_BUSY) != FLASH_STATUS_BUSY);
}