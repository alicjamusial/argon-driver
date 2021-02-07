#include <iostream>

#include "flash_controller/FlashController.hpp"
#include <charconv>
#include <ftd2xx.h>

std::uint32_t listDevices()
{
    DWORD devicesCount = 0;

    FT_CreateDeviceInfoList(&devicesCount);
    auto devices = new FT_DEVICE_LIST_INFO_NODE[devicesCount];

    auto r = FT_GetDeviceInfoList(devices, &devicesCount);
    std::cout << "Devices found: " << devicesCount << std::endl;

    for(auto i = 0; i < devicesCount; i++)
    {
        std::cout << "Device " << i << ":" << devices[i].Description << "\t"
                  << devices[i].SerialNumber << std::endl;
    }

    return devicesCount;
}

std::uint32_t listChannels()
{
    std::uint32_t channels;
    SPI_GetNumChannels(&channels);
    printf("\nAvailable SPI channels found: %d\n", channels);

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
    if(argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        std::cout << "Usage: argon.exe (or use argon-cli.exe for CLI experience)" << std::endl;

        return 0;
    }

    Init_libMPSSE();

    if(listDevices() > 0)
    {
        std::uint32_t channels = listChannels();
        std::uint32_t channelNo = 0;
        std::string channelText;

        std::cout << "\nChoose channel: ";
        std::cin >> channelText;
        std::from_chars(channelText.data(), channelText.data() + channelText.size(), channelNo);

        if(channelNo < channels)
        {
            std::cout << "\nUsing channel: " << channelNo << "\n" << std::endl;

            // Initialize SPI
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
                             "special|erase_chip|status_register2|status_register3|status_"
                             "register4|rems): ";
                std::cin >> action;
                std::cout << "\n";

                if(action == "id")
                {
                    flashController.ReadId();
                }

                if(action == "rems")
                {
                    flashController.ReadRems();
                }

                if(action == "status")
                {
                    flashController.ReadStatus();
                }

                if(action == "status_register")
                {
                    flashController.ReadStatusRegister();
                }

                if(action == "status_register2")
                {
                    flashController.ReadStatusRegister2();
                }

                if(action == "status_register3")
                {
                    flashController.ReadStatusRegister3();
                }

                if(action == "status_register4")
                {
                    flashController.ReadStatusRegister4();
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
                        offset, reinterpret_cast<const uint8_t*>(locationText.data()), locationText.size());
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
            std::cout << "\nWrong channel: " << channelNo << ". It has to be a value of detected "
                      << channels << "channels." << std::endl;
        }
    }
    else
    {
        std::cout << "\nNo devices found. Please connect your Argon to the USB.";
    }

    std::cout << "\nExiting.";
    return 0;
}