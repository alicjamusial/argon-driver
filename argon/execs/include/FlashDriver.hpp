#pragma once

#include "SPI.hpp"
#include <algorithm>
#include <array>

using namespace spi;

namespace flash
{
    struct Id
    {
        const std::uint8_t Manufacturer;
        const std::uint8_t MemoryType;
        const std::uint8_t Capacity;
        const std::array<std::uint8_t, 17> Data;

        Id(std::uint8_t manufacturer, std::uint8_t type, std::uint8_t capacity, std::array<uint8_t, 17> data) :
            Manufacturer(manufacturer),
            MemoryType(type),
            Capacity(capacity),
            Data(data)
        {
        }
    };

    enum Status
    {
        StatusRegisterWriteDisabled = 1 << 7,
        ProtectedAreaFromBottom = 1 << 5,
        WriteEnabled = 1 << 1,
        WriteInProgress = 1 << 0
    };

    enum FlagStatus
    {
        ProgramEraseControllerReady = 1 << 7,
        EraseSuspended = 1 << 6,
        EraseError = 1 << 5,
        ProgramError = 1 << 4,
        VPPDisable = 1 << 3,
        ProgramSuspended = 1 << 2
    };

    enum class CommandType
    {
        // Identification
        ReadId = 0x9F,
        ReadId2 = 0x90,

        // Register reads
        ReadStatusRegister = 0x05,
        ReadStatusRegister2 = 0x35,
        ReadStatusRegister3 = 0x15,
        ReadLockRegister = 0xE8,
        ReadFlagStatusRegister = 0x70,
        ReadNonvolatileConfigurationRegister = 0xB5,
        ReadVolatileConfigurationRegister = 0x85,

        // Reads
        ReadMemory = 0x03,

        // Write operations
        WriteEnable = 0x06,
        WriteDisable = 0x04,

        // Program operations
        ProgramMemory = 0x02,

        // Erase operations
        EraseSubsector = 0x20, // 4KB
        EraseSector = 0xD8,    // 64KB
        EraseChip = 0xC7
    };

    class FlashDriver
    {
    public:
        FlashDriver(SPI& spi);

        void Command(CommandType commandCode, std::uint8_t* response, std::size_t responseSize) const;

        Id ReadId() const;
        Status StatusRegister() const;
        FlagStatus FlagStatusRegister() const;

        void ReadMemory(uint32_t offset, std::uint8_t* buffer, std::size_t size) const;

        void ProgramMemory(uint32_t offset, const std::uint8_t* data, std::size_t size) const;
        void EraseSubsector(uint32_t offset) const;
        void EraseChip() const;

    private:
        SPI _spi;

        void CommandNoSelect(CommandType commandCode, std::uint8_t* response, std::size_t responseSize) const;

        void WriteCommand(CommandType byte) const;
        void WriteAddress(std::uint32_t offset) const;
        void WaitForStatus(Status status, bool state) const;

        void WriteEnable() const;
        void WriteDisable() const;

        void WaitBusy() const;
    };
}
