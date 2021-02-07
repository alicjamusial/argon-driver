#pragma once
#include "CLI/CLI.hpp"
#include "flash_controller/FlashDriver.hpp"

class GlobalOptions
{
public:
    GlobalOptions(const GlobalOptions&) = delete;

    GlobalOptions(CLI::App& app);

    SPI ConnectToFlash();

    const std::string& Serial() const;

private:
    std::int32_t FindChannelBySerialNumber(const char* serialNumber);

    std::string _serial;
};

inline const std::string& GlobalOptions::Serial() const
{
    return _serial;
}