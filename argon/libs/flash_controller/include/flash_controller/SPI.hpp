#pragma once
#include <Windows.h>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include <string>

#include "ftd2xx.h"
#include "mpsse/libMPSSE_spi.h"

extern const std::string FTErrorTypes[];

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
        std::runtime_error(std::string("FT error = ") + FTErrorTypes[status])
    {
    }

private:
    FT_STATUS _status;
};

namespace spi
{
    class SPI
    {
    public:
        SPI(std::uint32_t channel, std::uint32_t frequency, SPIBusOptions busOptions);
        ~SPI();

        FT_HANDLE Handle() const
        {
            return this->_handle;
        }

        void ChipSelect(bool state) const;

        void Write(const std::uint8_t* data, std::size_t size) const;
        void Read(std::uint8_t* data, std::size_t size) const;

        std::uint8_t Exchange(std::uint8_t input);

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
}
