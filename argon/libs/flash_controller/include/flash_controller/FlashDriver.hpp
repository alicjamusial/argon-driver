#pragma once

#include "SPI.hpp"
#include <algorithm>
#include <array>
#include <optional>
#include <vector>

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
            Manufacturer(manufacturer), MemoryType(type), Capacity(capacity), Data(data)
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
    enum class CommandType
    {
        // Identification
        ReadId = 0x9F,
        ReadRems = 0x90,

        // Register reads
        ReadStatusRegister1 = 0x05,
        ReadStatusRegister2 = 0x35,
        ReadStatusRegister3 = 0x15,
        ReadStatusRegister4 = 0x07,
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

    struct FlashParameter
    {
        std::uint16_t ParameterId;
        std::uint16_t Size;
        std::uint32_t Offset;
    };

    struct SectorType
    {
        std::uint8_t Index;
        std::uint8_t EraseOpcode;
        std::uint32_t Size;
    };

    struct FlashInformation
    {
        std::vector<SectorType> SectorTypes;
        std::size_t SizeInBytes;
    };

    class FlashDriver
    {
    public:
        FlashDriver(SPI& spi);

        void Command(CommandType commandCode, std::uint8_t* response, std::size_t responseSize) const;

        Id ReadId() const;
        std::array<std::uint8_t, 16> ReadRems() const;
        std::uint8_t FlagStatusRegister() const;
        std::uint8_t StatusRegister2() const;
        std::uint8_t StatusRegister3() const;
        std::uint8_t StatusRegister4() const;
        std::uint8_t StatusRegister1() const;
        void ReadMemory(std::uint32_t offset, std::uint8_t* buffer, std::size_t size) const;
        void ProgramMemory(std::uint32_t offset, const std::uint8_t* data, std::size_t size) const;
        void EraseSubsector(std::uint32_t offset) const;
        void EraseChip() const;

        Status ParseStatusRegister() const;

        std::vector<FlashParameter> ReadSFDP();
        std::vector<std::uint8_t> ReadParameter(const FlashParameter& parameter);

        std::optional<FlashInformation> ReadFlashInformation();

    private:
        SPI& _spi;

        void CommandNoSelect(CommandType commandCode, std::uint8_t* response, std::size_t responseSize) const;

        void WriteCommand(CommandType byte) const;
        void WriteAddress(std::uint32_t offset) const;
        void WaitForStatus(Status status, bool state) const;

        void WriteEnable() const;
        void WriteDisable() const;

        void WaitBusy() const;
    };
}
