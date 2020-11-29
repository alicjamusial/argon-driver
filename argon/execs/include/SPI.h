#pragma once
#include <Windows.h>
#include <cstdint>
#include <exception>
#include <ftd2xx.h>
#include <mpsse/libMPSSE_spi.h>
#include <stdexcept>
#include <string>

enum SPIBusOptions
{
    ChipSelectLine3 = SPI_CONFIG_OPTION_CS_DBUS3,
    ChipSelectActiveLow = SPI_CONFIG_OPTION_CS_ACTIVELOW,
    SpiMode0 = SPI_CONFIG_OPTION_MODE0,
};

inline SPIBusOptions operator|(SPIBusOptions a, SPIBusOptions b)
{
    return static_cast<SPIBusOptions>(static_cast<int>(a) | static_cast<int>(b));
}

class FTError : public std::runtime_error
{
public:
    explicit FTError(FT_STATUS status) :
        std::runtime_error(std::string("FT error = ") + std::to_string(status))
    {
    }

private:
    FT_STATUS _status;
};

class SPI
{
public:
    SPI(uint32_t channel, uint32_t frequency, SPIBusOptions busOptions);
    ~SPI();

    FT_HANDLE Handle() const
    {
        return this->_handle;
    }

    void ChipSelect(bool state) const;

    void Write(const uint8_t* data, size_t size) const;
    void Read(uint8_t* data, size_t size) const;

    std::uint8_t Exchange(const std::uint8_t input);

public:
    static uint32_t FindChannelByPort(const char port);

private:
    FT_HANDLE _handle;

    static void Check(FT_STATUS status);
};

class SPISelectSlave
{
public:
    SPISelectSlave(const SPI& spi) : _spi(spi)
    {
        _spi.ChipSelect(true);
    }

    ~SPISelectSlave()
    {
        _spi.ChipSelect(false);
    }

private:
    const SPI& _spi;
};