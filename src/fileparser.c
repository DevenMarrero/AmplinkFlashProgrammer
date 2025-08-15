#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "fileparser.h"

#define MAX_LINE_LENGTH 512
#define MAX_DATA_BYTES 255

uint8_t hex_to_byte(const unsigned char *hex){
    uint8_t val = 0;
    for (int i = 0; i < 2; i++){
        uint8_t c = hex[i];
        val = val << 4;
        if (c >= '0' && c <= '9')        
            val |= (c - '0');
        else if (c >= 'A' && c <= 'F')
            val |= (c - 'A' + 10);
        else if (c >= 'a' && c <= 'f')
            val |= (c - 'a' + 10);
    }
    return val;
}

FT_STATUS fileparser_stream_intel_hex(const char *filename, FT_STATUS (*programmer_callback)(uint32_t addr, const uint8_t *data, uint8_t len)){
    FILE *file = fopen(filename, "r");
    if (!file){
        printf("could not open file '%s': ", filename);
        return FT_IO_ERROR;
    }

    char line[MAX_LINE_LENGTH];
    uint32_t ext_addr = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)){
        // remove newline
        line[strcspn(line, "\n\r")] = '\0';

        // start code
        if (line[0] != ':') continue;

        // header bytes
        uint8_t byte_count = hex_to_byte(&line[1]);
        uint16_t address = (hex_to_byte(&line[3]) << 8) | hex_to_byte(&line[5]);
        uint8_t data_type = hex_to_byte(&line[7]);
        uint8_t checksum = hex_to_byte(&line[0x09 + (byte_count << 1)]);
        
        uint8_t calc_checksum = byte_count + (address >> 8) + (address & 0xFF) + data_type;

        // data bytes
        uint8_t data[MAX_DATA_BYTES];
        for (int i = 0; i < byte_count; i++){
            uint8_t val = hex_to_byte(&line[9 + i * 2]);
            data[i] = hex_to_byte(&line[9 + i * 2]);
            calc_checksum += val;
        }

        // verify checksum
        calc_checksum = (~calc_checksum + 1) & 0xFF; // two's complement
        if (calc_checksum != checksum){
            fprintf(stderr, "checksum error in file '%s' line: %s\n", filename, line);
            fprintf(stderr, "calculated: %02X, expected: %02X\n", calc_checksum, checksum);
            fclose(file);
            return FT_INVALID_PARAMETER;
        }
        
        // process data
        if (data_type == 0x00){
            uint32_t full_addr = (ext_addr << 16) | address;
            FT_STATUS ftStatus = programmer_callback(full_addr, data, byte_count);
            if (ftStatus != FT_OK){
                fprintf(stderr, "Failed to write to flash at address 0x%04X\n", address);
                fclose(file);
                return ftStatus;
            }

        } else if (data_type == 0x01){
            break; // EOF
        } else if (data_type == 0x04){
            ext_addr = (data[0] << 8) | data[1];
        } 
    }
    fclose(file);
    return FT_OK;
}