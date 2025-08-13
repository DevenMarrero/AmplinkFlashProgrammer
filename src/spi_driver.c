#include "spi_driver.h"
#include "utils.h"
#include "libmpsse_spi.h"


FT_STATUS spi_driver_init(ftd_channel_t deviceNumber, FT_HANDLE *pHandle){
    FT_STATUS ftStatus;
    ChannelConfigSPI channelConfSPI;

    RETURN_IF_ERROR(SPI_OpenChannel(deviceNumber, pHandle));

    memset(&channelConfSPI, 0, sizeof(channelConfSPI));
    channelConfSPI.ClockRate = 100000;
    channelConfSPI.LatencyTimer = 255;
    channelConfSPI.configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_ACTIVELOW;
    channelConfSPI.Pin = 0xFFFFFFFF; // all pins output high on init/close

    return SPI_InitChannel(*pHandle, &channelConfSPI);
}


FT_STATUS spi_driver_setCS(FT_HANDLE ftHandle, spi_chip_select_t chipSelect){
    DWORD configOptions = SPI_CONFIG_OPTION_MODE0 | SPI_CONFIG_OPTION_CS_ACTIVELOW | chipSelect;
    return SPI_ChangeCS(ftHandle, configOptions);
}


FT_STATUS spi_driver_write(FT_HANDLE ftHandle, uint8_t *tx_buff, uint32_t numBytes){
    DWORD bytesTransferred;
    RETURN_IF_ERROR(SPI_Write(ftHandle, tx_buff, numBytes, &bytesTransferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES |
                                                            SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE |
                                                            SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE));
    if (bytesTransferred != numBytes) 
        return FT_OTHER_ERROR;
    return FT_OK;
}


FT_STATUS spi_driver_transfer(FT_HANDLE ftHandle, uint8_t *tx_buff, uint32_t num_write, uint8_t *rx_buff, uint32_t num_read){
    DWORD bytesTransferred;
    

    RETURN_IF_ERROR(SPI_Write(ftHandle, tx_buff, num_write, &bytesTransferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_ENABLE));
    if (bytesTransferred != num_write) 
        return FT_OTHER_ERROR;
    
    bytesTransferred = 0;
    RETURN_IF_ERROR(SPI_Read(ftHandle, rx_buff, num_read, &bytesTransferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES | SPI_TRANSFER_OPTIONS_CHIPSELECT_DISABLE));
    if (bytesTransferred != num_read) 
        return FT_OTHER_ERROR;
    return FT_OK;
}

FT_STATUS spi_driver_close(FT_HANDLE ftHandle){
    return SPI_CloseChannel(ftHandle);
}