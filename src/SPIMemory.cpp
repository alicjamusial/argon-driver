// SPIMemory.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "N25QDriver.h"
#include "SPI.h"
//#include "Size.h"
#include "mpsse/libMPSSE_spi.h"
#include <ftd2xx.h>

using namespace std;

constexpr int64_t operator"" KB(unsigned long long int value)
{
    return value * 1024;
}

constexpr inline int64_t operator"" MB(unsigned long long int value)
{
    return value * 1024 * 1024;
}

void listDevices()
{
    DWORD devicesCount = 0;

    FT_CreateDeviceInfoList(&devicesCount);

    auto devices = new FT_DEVICE_LIST_INFO_NODE[devicesCount];

    auto r = FT_GetDeviceInfoList(devices, &devicesCount);

    cout << "Devices count = " << devicesCount << endl;

    for(auto i = 0; i < devicesCount; i++)
    {
        cout << devices[i].Description << "\t" << devices[i].SerialNumber << endl;
    }
}

void readId(N25Q::Driver& device)
{
    auto id = device.ReadId();

    cout << "Manufacturer " << uppercase << hex << "0x" << (int)id.Manufacturer << endl
         << "Memory type " << uppercase << hex << "0x" << (int)id.MemoryType << endl
         << "Memory capacity " << uppercase << hex << "0x" << (int)id.Capacity << endl
         << "Data: ";

    for(auto b: id.Data)
    {
        cout << uppercase << hex << (int)b << " ";
    }

    cout << endl;
}

void readAllMemory(N25Q::Driver& device, const char* fileName)
{
    auto buffer = new std::array<uint8_t, 1MB>();

    ofstream out(fileName, ofstream::binary);

    for(uint32_t offset = 0MB; offset < 8MB; offset += buffer->size())
    {
        cout << "Reading range from " << uppercase << hex << offset << " to " << uppercase << hex
             << (offset + buffer->size() - 1) << endl;

        buffer->fill(0xCC);

        device.ReadMemory(offset, buffer->data(), buffer->size());

        out.write(reinterpret_cast<char*>(buffer->data()), buffer->size());
    }

    delete buffer;
}

void readStatus(N25Q::Driver& device)
{
    uint8_t status = device.StatusRegister();

    cout << "Status register " << uppercase << hex << (int)status << endl;

    uint8_t flag = device.FlagStatusRegister();

    cout << "Flag status register " << uppercase << hex << (int)flag << endl;
}

void writeSomething(N25Q::Driver& device)
{
    std::array<uint8_t, 256> data;
    for(auto i = 0; i < data.size(); i++)
    {
        data[i] = 255 - i;
    }

    device.ProgramMemory(2MB + 512KB, data.data(), data.size());
}

void eraseRange(N25Q::Driver& device, std::uint32_t start, std::uint32_t end)
{
    for(auto sector = start; sector < end; sector += 4KB)
    {
        std::cout << "0x" << hex << uppercase << sector << " " << sector + 4KB << std::endl;
        device.EraseSubsector(sector);
    }
}

void checkId(N25Q::Driver& device)
{
    uint32_t counter = 0;
    uint32_t failures = 0;

    std::cout << dec;

    while(true)
    {
        auto id = device.ReadId();

        bool isOk = (id.Manufacturer == 0x20) && (id.MemoryType == 0xBA) && (id.Capacity == 0x18);

        counter++;

        if(!isOk)
        {
            failures++;

            cout << "Failures " << (double)failures / (double)counter * 100 << "% (" << failures
                 << "/" << counter << ")" << endl;
        }

        if((counter % 1000) == 0)
        {
            cout << "Failures " << (double)failures / (double)counter * 100 << "% (" << failures
                 << "/" << counter << ")" << endl;
        }

        // Sleep(10);
    }
}

static constexpr auto BV(std::uint8_t pos)
{
    return 1 << pos;
}

int main(int argc, char** argv)
{
    Init_libMPSSE();

    //    const char* serialNumber = "FTXPDDZ8A";// "FTZ3CQ6ZB";
    //
    //    int channelNo = SPI::FindChannelBySerialNumber(serialNumber);
    //    printf("Hello World\n");
    //
    //    DWORD version;
    //    auto r = FT_GetLibraryVersion(&version);
    //
    //    printf("result = %d\n", r);
    //    printf("version = 0x%X\n", version);
    //
    //    cout << "Using channel " << channelNo << endl;
    //
    //    SPI spi(channelNo,  4 * 1000 * 1000, ChipSelectLine3 | ChipSelectActiveLow | SpiMode0);
    //
    //    spi.ChipSelect(false);
    //
    //	N25Q::Driver flash{spi};
    //
    //	if(argc == 2 && strcmp(argv[1], "id") == 0)
    //	{
    //		readId(flash);
    //		return 0;
    //	}
    //
    //	if(argc == 2 && strcmp(argv[1], "check") == 0)
    //	{
    //		checkId(flash);
    //		return 0;
    //	}
    //
    //	if(argc == 3 && strcmp(argv[1], "read") == 0)
    //	{
    //		std::cout << "Reading memory to " << argv[2] << std::endl;
    //		readAllMemory(flash, argv[2]);
    //		return 0;
    //	}
    //
    //	if(argc == 2 && strcmp(argv[1], "erase_special") == 0)
    //	{
    //		std::cout << "Erasing spacial " << std::endl;
    //		eraseRange(flash, 0x2000, 0x5FE000);
    //		return 0;
    //	}
    //
    //	if(argc == 2 && strcmp(argv[1], "erase_normal") == 0)
    //	{
    //		std::cout << "Erasing normal " << std::endl;
    //		eraseRange(flash, 0x600000, 0x7FC000);
    //		return 0;
    //	}

    FT_STATUS ftStatus;
    FT_DEVICE_LIST_INFO_NODE* devInfo;
    DWORD numDevs; // create the device information list

    ftStatus = FT_CreateDeviceInfoList(&numDevs);

    if(ftStatus == FT_OK)
    {
        printf("Number of devices is %d\n", numDevs);
    }
    if(numDevs > 0)
    {
        devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE) * numDevs);
        ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
        if(ftStatus == FT_OK)
        {
            for(int i = 0; i < numDevs; i++)
            {
                printf("Dev %d:\n", i);
                printf("  Flags=0x%x\n", devInfo[i].Flags);
                printf(" Type=0x%x\n", devInfo[i].Type);
                printf("  ID=0x%x\n", devInfo[i].ID);
                printf(" LocId=0x%x\n", devInfo[i].LocId);
                printf("  SerialNumber=%s\n", devInfo[i].SerialNumber);
                printf(" Description=%s\n", devInfo[i].Description);
                printf("  ftHandle=0x%x\n", devInfo[i].ftHandle);
            }
        }
    }

    std::cout << "SPIMemory id" << std::endl;
    std::cout << "SPIMemory read <file>" << std::endl;
    return 1;

    return 0;
}
