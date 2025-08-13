#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ftd2xx.h"


/*!
 * @param filename Name of the file to parse data from
 * @param programmer_callback Callback function from programmer.c to write processed data
*/
FT_STATUS fileparser_stream_intel_hex(const char *filename, FT_STATUS (*programmer_callback)(uint32_t addr, const uint8_t *data, uint8_t len));

#endif