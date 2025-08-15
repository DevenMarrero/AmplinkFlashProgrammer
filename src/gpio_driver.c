#include "gpio_driver.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FT_STATUS gpio_driver_init(ftd_channel_t deviceChannel, FT_HANDLE *pHandle){
    FT_STATUS ftStatus;
    RETURN_IF_ERROR(FT_Open(deviceChannel, pHandle));
    // all pins output in sync mode
    RETURN_IF_ERROR(FT_SetBitMode(*pHandle, 0xFF, FT_BITMODE_ASYNC_BITBANG));

    RETURN_IF_ERROR(FT_SetLatencyTimer(*pHandle, 1)); // 1ms latency
    RETURN_IF_ERROR(FT_SetTimeouts(*pHandle, 200, 200)); // 200ms timouts
    //FT_SetUSBParameters(ftHandle, 64,64); // small usb transfer size
    //FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);
    RETURN_IF_ERROR(FT_SetDataCharacteristics(*pHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE));
    return FT_Purge(*pHandle, FT_PURGE_RX | FT_PURGE_TX);
}


FT_STATUS gpio_driver_read_port(FT_HANDLE ftHandle, unsigned char *value){
    DWORD bytesRead, bytesAvailable;
    FT_STATUS ftStatus;
    DWORD waitTime = 0;
    const DWORD maxWaitTime = 100; // max wait of 100ms

    // purge rx buffer
    RETURN_IF_ERROR(FT_Purge(ftHandle, FT_PURGE_RX));
    do{
        RETURN_IF_ERROR(FT_GetQueueStatus(ftHandle, &bytesAvailable));
        if (bytesAvailable >= 1){
            break;
        }
        Sleep(1); 
        waitTime += 1;
    } while (waitTime < maxWaitTime);
    
    if (bytesAvailable == 0){
        printf("No data after %dms", maxWaitTime);
        *value = 0;
        return FT_OTHER_ERROR;
    }

    // read one byte from port
    RETURN_IF_ERROR(FT_Read(ftHandle, value, 1, &bytesRead));
    if (bytesRead != 1){
        printf("Failed to read 1 byte, read %d\n", bytesRead);
        return FT_OTHER_ERROR;
    }
    return FT_OK;
}


uint8_t gpio_driver_read_pin(FT_HANDLE ftHandle, uint8_t pin){
    unsigned char state;
    FT_STATUS ftStatus = gpio_driver_read_port(ftHandle, &state);
    if (ftStatus != FT_OK){
        printf("gpio_read_port failed: %d", ftStatus);
        return 0;
    }
    return ((state & pin) != 0);
}


FT_STATUS gpio_driver_write_port(FT_HANDLE ftHandle, unsigned char value){
    FT_STATUS ftStatus;
    DWORD bytesWritten;

    // write byte
    RETURN_IF_ERROR(FT_Write(ftHandle, &value, 1, &bytesWritten));

    if (bytesWritten != 1){
        printf("Failed to write 1 byte, wrote %d\n", bytesWritten);
        return FT_OTHER_ERROR;
    }

    // flush tx buffer
    return FT_Purge(ftHandle, FT_PURGE_TX);
}


FT_STATUS gpio_driver_write_pin(FT_HANDLE ftHandle, uint8_t pin, uint8_t value){
    unsigned char state;
    RETURN_IF_ERROR(gpio_driver_read_port(ftHandle, &state));
    if (value)
        state |= pin;
    else
        state &= ~pin;
    return gpio_driver_write_port(ftHandle, state);
}


FT_STATUS gpio_driver_set_port_direction(FT_HANDLE ftHandle, uint8_t dir){
    return FT_SetBitMode(ftHandle, dir, FT_BITMODE_SYNC_BITBANG);
}


FT_STATUS gpio_driver_close(FT_HANDLE ftHandle){
    return FT_Close(ftHandle);
}