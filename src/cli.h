#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// strcuture to hold parsed args
typedef struct {
    char *file1_name;
    char *file2_name;
    char *file3_name;
    char *file4_name;
    unsigned char i2c_addr;
} Args;

int parse_args(int argc, char *argv[], Args *args);