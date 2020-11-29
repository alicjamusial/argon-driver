#include "mpsse/libMPSSE_spi.h"
#include <Windows.h>
#include <algorithm>
#include <ftd2xx.h>
#include <iostream>

#include "SPI.h"

SPI::SPI(uint32_t channel, uint32_t frequency, SPIBusOptions busOptions)
{
    Check(SPI_OpenChannel(channel, &this->_handle));

    FT_SetUSBParameters(this->_handle, 0xFFFF, 0xFFFF);

    ChannelConfig config;

    config.ClockRate = frequency;
    config.LatencyTimer = 1;
    config.configOptions = busOptions;

    Check(SPI_InitChannel(this->_handle, &config));
}

SPI::~SPI()
{
    SPI_CloseChannel(this->_handle);
}

void SPI::ChipSelect(bool state) const
{
    Check(SPI_ToggleCS(this->_handle, state));
}

void SPI::Write(const uint8_t* data, size_t size) const
{
    auto position = const_cast<uint8_t*>(data);

    while(size > 0)
    {
        uint32_t chunkSize = std::min(size, static_cast<size_t>(0x10000));

        uint32 transfered;
        Check(SPI_Write(this->_handle, position, chunkSize, &transfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES));

        size -= transfered;
        position += transfered;
    }
}

void SPI::Read(uint8_t* data, size_t size) const
{
    if(data == nullptr)
    {
        return;
    }

    auto position = data;

    while(size > 0)
    {
        uint32_t chunkSize = std::min(size, static_cast<size_t>(0x10000));

        uint32 transfered;
        Check(SPI_Read(this->_handle, position, chunkSize, &transfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES));

        size -= transfered;
        position += transfered;
    }
}

std::uint8_t SPI::Exchange(const std::uint8_t input)
{
    std::uint8_t output = 55;
    uint32 transferred;

    SPI_ReadWrite(
        this->_handle, &output, const_cast<std::uint8_t*>(&input), 1, &transferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES);

    return output;
}

uint32_t SPI::FindChannelByPort(const char port)
{
    uint32 channels;
    SPI_GetNumChannels(&channels);
    printf("Channels: %d\n", channels);
    for(int i = 0; i < channels; i++)
    {
        FT_DEVICE_LIST_INFO_NODE info;

        SPI_GetChannelInfo(i, &info);
        //
        //        printf("\n\nDev %d:\n", i);
        //        printf("  Flags=0x%x\n", info.Flags);
        //        printf(" Type=0x%x\n", info.Type);
        //        printf("  ID=0x%x\n", info.ID);
        //        printf(" LocId=0x%x\n", info.LocId);
        //        printf("  SerialNumber=%s\n", info.SerialNumber);
        //        printf(" Description=%s\n", info.Description);
        //        printf("  ftHandle=0x%x\n", info.ftHandle);

        std::size_t length = strlen(info.Description);
        if(info.Description[length - 1] == port)
        {
            return i;
        }
    }

    return -1;
}

void SPI::Check(FT_STATUS status)
{
    if(status == FT_OK)
    {
        return;
    }

    throw FTError(status);
}
