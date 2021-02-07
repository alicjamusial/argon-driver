#include "mpsse/libMPSSE_spi.h"
#include <Windows.h>
#include <algorithm>
#include <ftd2xx.h>
#include <iostream>

#include "flash_controller/SPI.hpp"

const std::string FTErrorTypes[] = {
    "FT_OK",
    "FT_INVALID_HANDLE",
    "FT_DEVICE_NOT_FOUND",
    "FT_DEVICE_NOT_OPENED",
    "FT_IO_ERROR",
    "FT_INSUFFICIENT_RESOURCES",
    "FT_INVALID_PARAMETER",
    "FT_INVALID_BAUD_RATE",

    "FT_DEVICE_NOT_OPENED_FOR_ERASE",
    "FT_DEVICE_NOT_OPENED_FOR_WRITE",
    "FT_FAILED_TO_WRITE_DEVICE",
    "FT_EEPROM_READ_FAILED",
    "FT_EEPROM_WRITE_FAILED",
    "FT_EEPROM_ERASE_FAILED",
    "FT_EEPROM_NOT_PRESENT",
    "FT_EEPROM_NOT_PROGRAMMED",
    "FT_INVALID_ARGS",
    "FT_NOT_SUPPORTED",
    "FT_OTHER_ERROR",
    "FT_DEVICE_LIST_NOT_READY",

};

namespace spi
{
    SPI::SPI(uint32_t channel, std::uint32_t frequency, SPIBusOptions busOptions)
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
            std::uint32_t chunkSize = std::min(size, static_cast<size_t>(0x10000));

            std::uint32_t transferred;
            Check(SPI_Write(
                this->_handle, position, chunkSize, &transferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES));

            size -= transferred;
            position += transferred;
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
            std::uint32_t chunkSize = std::min(size, static_cast<size_t>(0x10000));

            std::uint32_t transfered;
            Check(SPI_Read(
                this->_handle, position, chunkSize, &transfered, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES));

            size -= transfered;
            position += transfered;
        }
    }

    std::uint8_t SPI::Exchange(const std::uint8_t input)
    {
        std::uint8_t output = 55;
        std::uint32_t transferred;

        SPI_ReadWrite(
            this->_handle, &output, const_cast<std::uint8_t*>(&input), 1, &transferred, SPI_TRANSFER_OPTIONS_SIZE_IN_BYTES);

        return output;
    }

    void SPI::Check(FT_STATUS status)
    {
        if(status == FT_OK)
        {
            return;
        }

        throw FTError(status);
    }

}
