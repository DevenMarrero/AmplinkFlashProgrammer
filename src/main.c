#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "ftd2xx.h"
//#include "libmpsse_i2c.h"
//#include "libmpsse_SPI.h"
#define APP_CHECK_STATUS(exp) {if(exp!=FT_OK){printf("%s:%d:%s(): status(0x%x) \
	!= FT_OK\n",__FILE__, __LINE__, __FUNCTION__,exp);}else{;}};


DWORD channels;
FT_HANDLE ftHandle;


int main() {
    FT_STATUS status = FT_OK;
    FT_DEVICE_LIST_INFO_NODE *devList;
    uint32_t i;

    DWORD numDevs;

    status = FT_CreateDeviceInfoList(&numDevs);

    if (numDevs > 0) {
        // allocate storage for list based on numDevs
        devList = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
        // get the device information list
        status = FT_GetDeviceInfoList(devList,&numDevs);
        if (status == FT_OK) {
            for (int i = 0; i < numDevs; i++) {
            printf("Dev %d:\n",i);
            printf(" Flags=0x%x\n",devList[i].Flags);
            printf(" Type=0x%x\n",devList[i].Type);
            printf(" ID=0x%x\n",devList[i].ID);
            printf(" LocId=0x%x\n",devList[i].LocId);
            printf(" SerialNumber=%s\n",devList[i].SerialNumber);
            printf(" Description=%s\n",devList[i].Description);
            printf(" ftHandle=0x%x\n",devList[i].ftHandle);
            }
        }
    }


   
    return 0;
}