#include <iostream>

#include "FlashController.hpp"
#include "FlashDriver.hpp"
#include "SPI.hpp"
#include "mpsse/libMPSSE_spi.h"
#include <charconv>
#include <ftd2xx.h>

std::uint32_t listDevices()
{
    DWORD devicesCount = 0;

    FT_CreateDeviceInfoList(&devicesCount);
    auto devices = new FT_DEVICE_LIST_INFO_NODE[devicesCount];

    auto r = FT_GetDeviceInfoList(devices, &devicesCount);
    std::cout << "Devices count: " << devicesCount << std::endl;

    for(auto i = 0; i < devicesCount; i++)
    {
        std::cout << devices[i].Description << "\t" << devices[i].SerialNumber << std::endl;
    }

    return devicesCount;
}

std::uint32_t findChannelByPort(const char port)
{
    std::uint32_t channels;
    SPI_GetNumChannels(&channels);
    printf("\nAvailable SPI channels count: %d\n", channels);
    for(auto i = 0; i < channels; i++)
    {
        FT_DEVICE_LIST_INFO_NODE info;
        SPI_GetChannelInfo(i, &info);

        //        std::cout << i << ": " << devices[i].Description << "\t" << devices[i].SerialNumber << std::endl;

        printf("\n\nDev %d:\n", i);
        printf("  Flags=0x%x\n", info.Flags);
        printf(" Type=0x%x\n", info.Type);
        printf("  ID=0x%x\n", info.ID);
        printf(" LocId=0x%x\n", info.LocId);
        printf("  SerialNumber=%s\n", info.SerialNumber);
        printf(" Description=%s\n", info.Description);
        printf("  ftHandle=0x%x\n", info.ftHandle);

        std::size_t length = strlen(info.Description);
        if(info.Description[length - 1] == port)
        {
            return i;
        }
    }

    return -1;
}

std::uint32_t listChannels()
{
    std::uint32_t channels;
    SPI_GetNumChannels(&channels);
    printf("\nAvailable SPI channels count: %d\n", channels);
    for(auto i = 0; i < channels; i++)
    {
        FT_DEVICE_LIST_INFO_NODE info;
        SPI_GetChannelInfo(i, &info);

        std::cout << i << ": " << info.Description << "\t" << info.SerialNumber << std::endl;
    }
    return channels;
}

int main(int argc, char** argv)
{
    Init_libMPSSE();

    if(listDevices() > 0)
    {
        // Console mode
        if(argc > 2)
        {
            std::uint32_t channels = listChannels();

            std::uint32_t channelNo = 0;

            std::string channelInput = argv[1];
            std::from_chars(channelInput.data(), channelInput.data() + channelInput.size(), channelNo);

            if(channelNo < channels)
            {
                std::cout << "\nUsing channel: " << channelNo << "\n" << std::endl;
                SPI spi(channelNo, 1 * 1000 * 1000, ChipSelectLine3 | ChipSelectActiveLow | SpiMode0);

                spi.ChipSelect(false);

                flash::FlashDriver flash{spi};
                flash::FlashController flashController{flash};

                flashController.ReadId();

                if(argc == 3 && strcmp(argv[2], "id") == 0)
                {
                    flashController.ReadId();
                }

                if(argc == 3 && strcmp(argv[2], "status") == 0)
                {
                    flashController.ReadStatus();
                }

                if(argc == 4 && strcmp(argv[2], "read") == 0)
                {
                    std::string location = argv[3];
                    flashController.ReadAllMemory(location.c_str());
                }

                if(argc == 3 && strcmp(argv[2], "write_random") == 0)
                {
                    flashController.WriteSomething();
                }

                if(argc == 5 && strcmp(argv[2], "write") == 0)
                {
                    std::string offsetText = argv[3];
                    std::string locationText = argv[4];

                    std::uint32_t offset = 0;
                    std::from_chars(offsetText.data(), offsetText.data() + offsetText.size(), offset);

                    flashController.Write(
                        offset, reinterpret_cast<const uint8_t*>(locationText.data()), locationText.size());
                }

                if(argc == 3 && strcmp(argv[2], "erase_chip") == 0)
                {
                    flashController.EraseChip();
                }

                if(argc == 2 && strcmp(argv[1], "erase_special") == 0)
                {
                    flashController.EraseRange(0x2000, 0x5FE000);
                }

                if(argc == 2 && strcmp(argv[1], "erase_normal") == 0)
                {
                    flashController.EraseRange(0x600000, 0x7FC000);
                }
            }
            else
            {
                std::cout << "\nWrong channel: " << channelNo << ". It has to be smaller than "
                          << channels << "." << std::endl;
            }
        }

        // Manual mode
        else
        {
            uint32_t channels = listChannels();

            uint32_t channelNo = 0;

            std::cout << "\nChoose channel: ";
            std::cin >> channelNo;

            if(channelNo < channels)
            {
                std::cout << "\nUsing channel: " << channelNo << "\n" << std::endl;
                SPI spi(channelNo, 1 * 1000 * 1000, ChipSelectLine3 | ChipSelectActiveLow | SpiMode0);

                spi.ChipSelect(false);

                flash::FlashDriver flash{spi};
                flash::FlashController flashController{flash};

                flashController.ReadId();

                std::string action;

                while(action != "exit")
                {
                    std::cout << "\nChoose action "
                                 "(exit|id|status|read|write|write_random|erase_normal|erase_"
                                 "special|erase_chip): ";
                    std::cin >> action;
                    std::cout << "\n";

                    if(action == "id")
                    {
                        flashController.ReadId();
                    }

                    if(action == "status")
                    {
                        flashController.ReadStatus();
                    }

                    if(action == "read")
                    {
                        std::string location;
                        std::cout << "\nChoose filename to read to: ";
                        std::cin >> location;
                        std::cout << "\n";

                        flashController.ReadAllMemory(location.c_str());
                    }

                    if(action == "write_random")
                    {
                        flashController.WriteSomething();
                    }

                    if(action == "write")
                    {
                        std::string offsetText;
                        std::string locationText;

                        std::cout << "\nOffset: ";
                        std::cin >> offsetText;
                        std::cout << "\nData to write: ";
                        std::cin >> locationText;
                        std::cout << "\n";

                        std::uint32_t offset = 0;
                        std::from_chars(offsetText.data(), offsetText.data() + offsetText.size(), offset);

                        flashController.Write(
                            offset,
                            reinterpret_cast<const uint8_t*>(locationText.data()),
                            locationText.size());
                    }

                    if(action == "erase_chip")
                    {
                        flashController.EraseChip();
                    }

                    if(action == "erase_special")
                    {
                        flashController.EraseRange(0x2000, 0x5FE000);
                    }

                    if(action == "erase_normal")
                    {
                        flashController.EraseRange(0x600000, 0x7FC000);
                    }
                }
            }
            else
            {
                std::cout << "\nWrong channel: " << channelNo << ". It has to be smaller than "
                          << channels << "." << std::endl;
            }
        }
    }
    else
    {
        std::cout << "\nNo devices found. Please connect your Argon to the USB.";
    }

    std::cout << "\nExiting.";
    return 0;
}