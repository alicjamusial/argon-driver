#include <iostream>

#include "FlashController.hpp"
#include "FlashDriver.hpp"
#include "SPI.hpp"
#include "mpsse/libMPSSE_spi.h"
#include <ftd2xx.h>

void listDevices()
{
    DWORD devicesCount = 0;

    FT_CreateDeviceInfoList(&devicesCount);
    auto devices = new FT_DEVICE_LIST_INFO_NODE[devicesCount];

    auto r = FT_GetDeviceInfoList(devices, &devicesCount);
    std::cout << "Devices count = " << devicesCount << std::endl;

    for(auto i = 0; i < devicesCount; i++)
    {
        std::cout << devices[i].Description << "\t" << devices[i].SerialNumber << std::endl;
    }
}

int main(int argc, char** argv)
{
    Init_libMPSSE();

    listDevices();

    //    try
    //    {
    //
    const char port = 'A'; // B

    //            int channelNo = SPI::FindChannelByPort(port);
    //
    //            DWORD version;
    //            auto r = FT_GetLibraryVersion(&version);
    //
    //            printf("result = %d\n", r);
    //            printf("version = 0x%X\n", version);

    int channelNo = 0;
    std::cout << "Using channel " << channelNo << std::endl;

    SPI spi(channelNo, 1 * 1000 * 1000, ChipSelectLine3 | ChipSelectActiveLow | SpiMode0);

    spi.ChipSelect(false);

    flash::FlashDriver flash{spi};

    flash::FlashController flashController{flash};
    flashController.ReadId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readId(flash);
    //        //    readStatus(flash);
    //
    //        //    if(argc == 2 && strcmp(argv[1], "id") == 0)
    //        //    {
    //        //        readId(flash);
    //        //        return 0;
    //        //    }
    //        //
    //        //    	if(argc == 2 && strcmp(argv[1], "check") == 0)
    //        //    	{
    //        //    checkId(flash);
    //        //    		return 0;
    //        //    	}
    //        //
    //        //    	if(argc == 3 && strcmp(argv[1], "read") == 0)
    //        //    	{
    //        //    		std::cout << "Reading memory to " << argv[2] << std::endl;
    //        //    		readAllMemory(flash, argv[2]);
    //        //    		return 0;
    //        //    	}
    //        //
    //        //    	if(argc == 2 && strcmp(argv[1], "erase_special") == 0)
    //        //    	{
    //        //    		std::cout << "Erasing spacial " << std::endl;
    //        //    		eraseRange(flash, 0x2000, 0x5FE000);
    //        //    		return 0;
    //        //    	}
    //        //
    //        //    	if(argc == 2 && strcmp(argv[1], "erase_normal") == 0)
    //        //    	{
    //        //    		std::cout << "Erasing normal " << std::endl;
    //        //    		eraseRange(flash, 0x600000, 0x7FC000);
    //        //    		return 0;
    //        //    	}
    //
    //        //    FT_STATUS ftStatus;
    //        //    FT_DEVICE_LIST_INFO_NODE* devInfo;
    //        //    DWORD numDevs; // create the device information list
    //        //
    //        //    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    //        //
    //        //    if(ftStatus == FT_OK)
    //        //    {
    //        //        printf("Number of devices is %d\n", numDevs);
    //        //    }
    //        //    if(numDevs > 0)
    //        //    {
    //        //        devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) *
    //        //        numDevs); ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs); if(ftStatus == FT_OK)
    //        //        {
    //        //            for(int i = 0; i < numDevs; i++)
    //        //            {
    //        //                printf("Dev %d:\n", i);
    //        //                printf("  Flags=0x%x\n", devInfo[i].Flags);
    //        //                printf(" Type=0x%x\n", devInfo[i].Type);
    //        //                printf("  ID=0x%x\n", devInfo[i].ID);
    //        //                printf(" LocId=0x%x\n", devInfo[i].LocId);
    //        //                printf("  SerialNumber=%s\n", devInfo[i].SerialNumber);
    //        //                printf(" Description=%s\n", devInfo[i].Description);
    //        //                printf("  ftHandle=0x%x\n", devInfo[i].ftHandle);
    //        //            }
    //        //        }
    //        //    }
    //        //
    //        //    std::cout << "SPIMemory id" << std::endl;
    //        //    std::cout << "SPIMemory read <file>" << std::endl;
    //        //    return 1;
    //    }
    //    catch(FTError& e)
    //    {
    //        printf("aaaaaaaaaaaaAA");
    //    }

    return 0;
}