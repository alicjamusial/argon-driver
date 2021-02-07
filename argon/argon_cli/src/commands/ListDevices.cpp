#include "commands/ListDevices.hpp"
#include "flash_controller/FlashDriver.hpp"

namespace commands
{
    ListDevices::ListDevices(CLI::App& app) :
        _cmd{app.add_subcommand("list", "List connected FTDI devices")}
    {
        _cmd->callback([this]() { Execute(); });
    }

    void ListDevices::Execute()
    {
        std::cout << "Listing devices..." << std::endl;
        Init_libMPSSE();

        DWORD devicesCount = 0;

        FT_CreateDeviceInfoList(&devicesCount);
        auto devices = new FT_DEVICE_LIST_INFO_NODE[devicesCount];

        auto r = FT_GetDeviceInfoList(devices, &devicesCount);
        std::cout << "Devices found: " << devicesCount << std::endl;

        for(auto i = 0; i < devicesCount; i++)
        {
            std::cout << "Device " << i << ": " << devices[i].Description
                      << "\t serial: " << devices[i].SerialNumber << std::endl;
        }

        std::uint32_t channels;
        SPI_GetNumChannels(&channels);
        printf("\nAvailable SPI channels found: %d\n", channels);

        for(auto i = 0; i < channels; i++)
        {
            FT_DEVICE_LIST_INFO_NODE info;
            SPI_GetChannelInfo(i, &info);

            std::cout << i << ": " << info.Description << "\t serial: " << info.SerialNumber << std::endl;
        }
    }
}