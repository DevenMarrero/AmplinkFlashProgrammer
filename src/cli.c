#include "cli.h"
// parse command line args

void print_help() {
    printf("Usage: ./OneBox_processor_programmer [OPTIONS]\n\n");
    printf("Optional named arguments:\n");
    printf("  -1=FILENAME    Path to versaclock file (default: flash_clk.hex)\n");
    printf("  -2=FILENAME    Path to flash_2A file (default: flash_2A.hex)\n");
    printf("  -3=FILENAME    Path to flash_3A flash file (default: flash_3A.hex)\n");
    printf("  -4=FILENAME    Path to flash_4A file (default: flash_4A.hex)\n");
    printf("  -i=0xHH         Clock i2c address (default: 0x6A)\n");
    printf("  -h              Show this help message\n");
}

int parse_args(int argc, char *argv[], Args *args) {
    int opt;
    char *endptr;

    // init
    args->file1_name = NULL;
    args->file2_name = NULL;
    args->file3_name = NULL;
    args->file4_name = NULL;
    args->i2c_addr = 0x00;

    // parse command line args
    while ((opt = getopt(argc, argv, "1:2:3:4:i:h:")) != -1){
        switch (opt) {
            case '1':
                args->file1_name = optarg;
                break;
            case '2':
                args->file2_name = optarg;
                break;
            case '3':
                args->file3_name = optarg;
                break;
            case '4':
                args->file4_name = optarg;
                break;
            case 'i':
                if(strncmp(optarg, "0x", 2) == 0 || strncmp(optarg, "0X", 2) == 0) {
                    args->i2c_addr = (unsigned char)strtol(optarg + 2, &endptr, 16);
                } else {
                    args->i2c_addr = (unsigned char)strtol(optarg, &endptr, 16);
                }
                if (*endptr != '\0' || strlen(optarg) > 4){
                    fprintf(stderr, "Invalid i2c address: %s (must be 0xFF or FF format)\n", optarg);
                    return 1;
                } 
                break;
            case 'h':
                print_help();
                return 1;
            default:
                fprintf(stderr, "Usage: %s [-f1 file1] [-f2 file2] [-f3 file3] [-b i2c_address]\n", argv[0]);
                return 1;
        }
    }
    return 0;
}