#pragma once

#include "SPI.h"
#include <array>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>
#include <algorithm>

namespace N25Q
{
    struct Id
    {
        const uint8_t Manufacturer;
        const uint8_t MemoryType;
        const uint8_t Capacity;
        const std::array<uint8_t, 17> Data;

        Id(uint8_t manufacturer, uint8_t type, uint8_t capacity, std::array<uint8_t, 17> data):
            Manufacturer(manufacturer), 
            MemoryType(type),
            Capacity(capacity),
            Data(data)
        {}
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

    class Driver
    {
    public:
        Driver(SPI& spi);
        void Command(uint8_t commandCode, uint8_t* response, size_t responseSize) const;

        Id ReadId() const;

        Status StatusRegister() const;

        FlagStatus FlagStatusRegister() const;

        void ReadMemory(uint32_t offset, uint8_t* buffer, size_t size) const;

        void ProgramMemory(uint32_t offset, const uint8_t* data, size_t size) const;
        void EraseSubsector(uint32_t offset) const;
        void EraseChip() const;
    private:
        SPI _spi;

        void CommandNoSelect(const uint8_t commandCode, uint8_t* response, size_t responseSize) const;

        void WriteByte(const uint8_t byte) const;
        void WriteAddress(const uint32_t offset) const;
        void WaitForStatus(const Status status, bool state) const;

        void WriteEnable() const;
        void WriteDisable() const;

        void WaitBusy() const;
    };
}
