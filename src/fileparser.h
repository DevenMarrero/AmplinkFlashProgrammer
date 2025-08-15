/*!
 * @file fileparser.h
 * @brief Intel HEX file parser interface for streaming data to a programmer function.
 *
 * Defines function for Intel HEX files and streaming the parsed data to a user-defined
 * callback function.
 *
 * The callback function provided by the caller is responsible for writing data to 
 * target memory.
 *
 * @see fileparser_stream_intel_hex
*/

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ftd2xx.h"


/*!
 * @brief Parses an Intel HEX file and streams data to a porgrammer callback.
 *
 * This function reads an Intel HEX file line by line, validates the checksum,
 * and processes the record type.
 *
 * Supports:
 * - **Data records (0x00)**: Calls programmer callback with 32-bit address and data bytes.
 * - **End of File Records (0x01)**: Terminates parsing.
 * - **Extended Address Records (0x04)**: Updates upper 16-bytes of address for extended addressing.
 *
 * **Example usage:**
 * @code
 * FT_STATUS program_flash(uint32_t addr, const uint8_t *data, uint8_t len) {
 *     // Custom logic to write data to flash memory
 * }
 *
 * int main() {
 *     FT_STATUS status = fileparser_stream_intel_hex("file.hex", write_to_flash);
 *     if (status != FT_OK) {
 *         fprintf(stderr, "Failed to parse and program the file.\n");
 *         return -1;
 *     }
 *     return 0;
 * }
 * @endcode
 *
 * @param[in] filename Path to the Intel HEX file to parse.
 * @param[in] programmer_callback Callback function that writes the parsed data to the target.
 *                                The function signature must be:
 *                                @code
 *                                FT_STATUS callback(uint32_t addr, const uint8_t *data, uint8_t len);
 *                                @endcode
 *
 * @return FT_STATUS Status of the operation
 * - **FT_OK** if the file was parsed and written successfully
 * - **FT_IO_ERROR** if the file could not be opened
 * - **FT_INVALID_PARAMETER** if a checksum error occurs
 * - Any error code returned by the programmer callback
 *
 * @see programmer.h for implementations of callback.
 */
FT_STATUS fileparser_stream_intel_hex(const char *filename, FT_STATUS (*programmer_callback)(uint32_t addr, const uint8_t *data, uint8_t len));

#endif