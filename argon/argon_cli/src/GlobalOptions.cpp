#include "GlobalOptions.hpp"

GlobalOptions::GlobalOptions(CLI::App& app) : _serial{"DUPA"}
{
    app.add_option("--serial", _serial, "Argon serial number");
}

SPI GlobalOptions::ConnectToFlash()
{
    Init_libMPSSE();

    auto channel = FindChannelBySerialNumber(Serial().c_str());
    if(channel >= 0)
    {
        SPI spi(static_cast<std::uint32_t>(channel), 1 * 1000 * 1000, ChipSelectLine3 | ChipSelectActiveLow | SpiMode0);
        spi.ChipSelect(false);

        return spi;
    }
    else
    {
        throw std::exception();
    }
}

std::int32_t GlobalOptions::FindChannelBySerialNumber(const char* serialNumber)
{
    std::uint32_t channels;
    SPI_GetNumChannels(&channels);
    for(int i = 0; i < channels; i++)
    {
        FT_DEVICE_LIST_INFO_NODE info;
        SPI_GetChannelInfo(i, &info);

        if(strcmp(info.SerialNumber, serialNumber) == 0)
        {
            return i;
        }
    }

    std::cout << "No serial number was chosen or no device with chosen serial number is connected. "
                 "Use 'list devices --list' to "
                 "list all available devices."
              << std::endl;

    return -1;
}