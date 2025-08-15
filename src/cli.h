/*! @file cli.h
 *  @brief Handles parsing and validation of CLI arguments.
 * 
 * @details
 * Error handling in the case of incorrect or missing arguments 
 * must be handled by the caller.
 * 
 * @date 2025-08-14
 * @author Deven Marrero
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*!
 * @struct Args
 * @brief Holds the parse command-line arguments for the application
 * 
 * This structure Stores input file names for the flash devices and the
 * clock I2C address provided by the user through command-line arguments.
*/
typedef struct {
    char *file1_name;       /*!< Input file for VersaClock configuration */
    char *file2_name;       /*!< Input file for Flash 2A programming */
    char *file3_name;       /*!< Input file for Flash 3A programming */
    char *file4_name;       /*!< Input file for Flash 4A programming */
    unsigned char i2c_addr; /*!< I2C address of the VersaClock device */
} Args;


/*!
 * @brief Parses the command-line arguments and populates an Args struct
 *
 * Processes the CLI arguments and stores filenames and addresses in the given
 * Args structure. Handles input validation and sanitization.
 * 
 * @note Arguments not provided by the user will be initialized to NULL
 * 
 * @param[in] argc Number of command-line arguments
 * @param[in] argv Array of command-line arguments
 * @param[out] args Pointer to an Args struct to populate
 * @return int 0 on success, on-zero on error
 */
int parse_args(int argc, char *argv[], Args *args);