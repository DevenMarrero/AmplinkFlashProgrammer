#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include "ftd2xx.h"

#include "programmer.h"
#include "fileparser.h"
#include "config.h"
#include "cli.h"
// #include "gpio_driver.h"
// #include "spi_driver.h"
// #include "i2c_driver.h"

#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
	!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);}else{;}};


static const char* chip_select_to_str(spi_chip_select_t cs){
    switch(cs){
        case SPI_CS_1:  return "1";
        case SPI_CS_2:  return "2";
        case SPI_CS_3:  return "3";
        case SPI_CS_4:  return "4";
        default:        return "?";
    }
}
    

int main(int argc, char *argv[]) {
    int i;
    FT_STATUS ftStatus;
    unsigned char writeVal;
    unsigned char readVal;
    Args args;

    // get cli args and set defaults
    if (parse_args(argc, argv, &args) != 0)
        return 1;
    if (!args.file1_name) args.file1_name = "clock.hex";
    if (!args.file2_name) args.file2_name = "flash_2A.hex";
    if (!args.file3_name) args.file3_name = "flash_3A.hex";
    if (!args.file4_name) args.file4_name = "flash_4A.hex";
    if (!args.i2c_addr)   args.i2c_addr = 0x6A;


    printf("Connecting to AmPLink...  ");
    // init device
    ftStatus = programmer_init();
    if (ftStatus != FT_OK){
        printf("AmPLink device not found\n");
        MessageBox(NULL,
                    "AmPLink device not found!",
                    "Warning",
                    MB_OK | MB_ICONERROR);
        return -1;
    }
    printf("Success!\n");


    // -- SPI STREAM ---------------------
    // select chip
    char **filenames[] = {&args.file2_name, &args.file3_name, &args.file4_name};
    spi_chip_select_t chipSelects[] = {SPI_CS_2, SPI_CS_3, SPI_CS_4};
    for (int i = 0; i < 3; i++){
        const char *filename = *filenames[i];
        spi_chip_select_t chipSelect = chipSelects[i];
        // select flash chip
        ftStatus = programmer_flash_select_chip(chipSelect); // verify that gpio is switching
        if (ftStatus != FT_OK){
            printf("\nChipselect failed: %s\n", chip_select_to_str(chipSelect));
            continue;
        }
        else printf("\nSet SPI chipSelect to: %s\n", chip_select_to_str(chipSelect));
    
        // set write enable
        ftStatus = programmer_flash_set_write_state(1);
        if (ftStatus != FT_OK){
            printf("Failed to enable flash write\n");
            continue;
        }
    
        // erase chip
        printf("Erasing flash...   ");
        ftStatus = programmer_flash_erase_chip();
        if (ftStatus != FT_OK) {
            printf("FAILED!\n");
            // unset write enable
            ftStatus = programmer_flash_set_write_state(0);
            if (ftStatus != FT_OK) printf("Failed to disable flash write\n");
            continue;
        }
        else printf("Success!  \n");
    
        // stream file to page flash callback
        printf("Programming flash...   ");
        ftStatus = fileparser_stream_intel_hex(filename, programmer_flash_write_page);
        if (ftStatus != FT_OK) {
            printf("FAILED!\n");
            continue;
        }
        else printf("Success!\n");
    
        // verify readback
    
        // unset write enable
        ftStatus = programmer_flash_set_write_state(0);
        if (ftStatus != FT_OK) printf("Failed to disable flash write\n");
    }


    // --- I2C ------------------
    ftStatus = programmer_clock_set_addr(args.i2c_addr);
    if (ftStatus != FT_OK) printf("\nFailed to set i2c address: 0x%0X\n", args.i2c_addr);
    else printf("\nSet i2c address: 0x%0X\n", args.i2c_addr);

    printf("Programming clock...   ");
    ftStatus = fileparser_stream_intel_hex(args.file1_name, programmer_clock_write_page);
    if (ftStatus != FT_OK) 
        printf("FAILED!: stream\n");
    else {
        // burn data
        ftStatus = programmer_clock_burn();
        if (ftStatus != FT_OK) printf("FAILED!: burn\n");
        else printf("Success!\n");
    }


    programmer_close();
    return 0;
}