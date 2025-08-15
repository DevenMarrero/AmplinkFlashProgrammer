#include "i2c_driver.h"
#include "utils.h"

#define I2C_DEVICE_BUFFER_SIZE  256

FT_STATUS i2c_driver_init(ftd_channel_t deviceNumber, FT_HANDLE *pHandle){
    FT_STATUS ftStatus;
    ChannelConfigI2C channelConfI2C;

    RETURN_IF_ERROR(I2C_OpenChannel(deviceNumber, pHandle));

    memset(&channelConfI2C, 0, sizeof(channelConfI2C));
    channelConfI2C.ClockRate = I2C_CLOCK_STANDARD_MODE;
    channelConfI2C.LatencyTimer = 255;
    channelConfI2C.Options = 0;

    return I2C_InitChannel(*pHandle, &channelConfI2C);
}


FT_STATUS i2c_driver_write(FT_HANDLE ftHandle, uint8_t deviceAddress, uint8_t *data, uint32_t numBytes){
    FT_STATUS ftStatus;
    DWORD bytesTransfered = 0;
    DWORD options;

    options = I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT;

    ftStatus = I2C_DeviceWrite(ftHandle, deviceAddress, numBytes, data, &bytesTransfered, options);
    if (bytesTransfered != numBytes){
        return FT_OTHER_ERROR;
    }
    return ftStatus;
}


FT_STATUS i2c_driver_read(FT_HANDLE ftHandle, uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, uint32_t numBytes){
    FT_STATUS ftStatus;
    uint8_t buffer[I2C_DEVICE_BUFFER_SIZE];
    uint32_t bytesToTransfer = 0;
    DWORD bytesTransfered = 0;
    DWORD options;
    uint32_t trials;

    options = I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT;
    buffer[bytesToTransfer++] = registerAddress;
    RETURN_IF_ERROR(I2C_DeviceWrite(ftHandle, deviceAddress, bytesToTransfer, buffer, &bytesTransfered, options));

    trials = 0;
    while (trials < 20){
        ftStatus = I2C_DeviceRead(ftHandle, deviceAddress, numBytes, data, &bytesTransfered, options);
        if (ftStatus == FT_OK) break;
        Sleep(100);
        trials++;
    }
    return ftStatus;
}


FT_STATUS i2c_driver_close(FT_HANDLE ftHandle){
    return I2C_CloseChannel(ftHandle);
}