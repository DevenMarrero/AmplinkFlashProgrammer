#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "ftd2xx.h"

#define DEBUG // remove this for no debug print

#ifdef DEBUG
  #define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
#endif

#define RETURN_IF_ERROR(call)                         \
    do {                                              \
        FT_STATUS status = (call);                    \
        if (status != FT_OK) {                        \
            DEBUG_PRINT("[ERROR] %s failed: %d\n",     \
                        #call, status);               \
            return status;                            \
        }                                             \
    } while (0)

#endif // UTILS_H