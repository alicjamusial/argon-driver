#include <iostream>

#include "N25QDriver.h"

using std::cout;
using std::endl;

enum Command
{
    ReadId = 0x9F,
    ReadStatusRegister = 0x05,
    ReadFlagStatusRegister = 0x70,
    ReadMemory = 0x03,
    WriteEnable = 0x06,
    WriteDisable = 0x05,
    ProgramMemory = 0x02,
    EraseSubsector = 0x20,
    EraseChip = 0xC7
};

N25Q::Driver::Driver(SPI& spi) : _spi(spi)
{
}

void N25Q::Driver::Command(uint8_t commandCode, uint8_t* response, size_t responseSize) const
{
    SPISelectSlave select(this->_spi);

    this->CommandNoSelect(commandCode, response, responseSize);
}

void N25Q::Driver::CommandNoSelect(const uint8_t commandCode, uint8_t* response, size_t responseSize) const
{
    this->_spi.Write(&commandCode, sizeof(uint8_t));
    this->_spi.Read(response, responseSize);
}


N25Q::Id N25Q::Driver::ReadId() const
{
    uint8_t response[20] = { 0 };

    this->Command(Command::ReadId, response, sizeof(response));

    std::array<uint8_t, 17> d;
    for(auto i = 3; i < 20; i++)
    {
        d[i - 3] = response[i];
    }

    return Id(response[0], response[1], response[2], d);
}

N25Q::Status N25Q::Driver::StatusRegister() const
{
    uint8_t status;
    this->Command(Command::ReadStatusRegister, &status, sizeof(status));

    return static_cast<N25Q::Status>(status);
}

N25Q::FlagStatus N25Q::Driver::FlagStatusRegister() const
{
    uint8_t status;
    this->Command(Command::ReadFlagStatusRegister, &status, sizeof(status));

    return static_cast<N25Q::FlagStatus>(status);
}

void N25Q::Driver::WaitBusy() const
{
    do
    {
        cout << "B";
        uint8_t status = 0;

        this->CommandNoSelect(Command::ReadStatusRegister, &status, sizeof(status));

        if ((status & Status::WriteInProgress) == 0)
        {
            break;
        }

        Sleep(100);

    } while (true);
}

void N25Q::Driver::ReadMemory(uint32_t offset, uint8_t* buffer, size_t size) const
{
    SPISelectSlave select(this->_spi);

    this->WriteByte(Command::ReadMemory);
    this->WriteAddress(offset);

    this->_spi.Read(buffer, size);
}

void N25Q::Driver::ProgramMemory(uint32_t offset, const uint8_t* data, size_t size) const
{
    this->WriteEnable();

    {
        SPISelectSlave select(this->_spi);

        this->WriteByte(Command::ProgramMemory);
        this->WriteAddress(offset);

        this->_spi.Write(data, size);

        this->WaitBusy();
    }

    this->WriteDisable();
}

void N25Q::Driver::EraseSubsector(uint32_t offset) const
{
    this->WriteEnable();

    {
        SPISelectSlave select(this->_spi);

        this->WriteByte(Command::EraseSubsector);
        this->WriteAddress(offset);
    }

    {
        SPISelectSlave select(this->_spi);

        this->WaitBusy();
    }

    this->WriteDisable();
}

void N25Q::Driver::EraseChip() const
{
    this->WriteEnable();

    {
        SPISelectSlave select(this->_spi);
        this->WriteByte(Command::EraseChip);
    }

    {
        SPISelectSlave select(this->_spi);

        this->WaitBusy();
    }

    this->WriteDisable();
}

void N25Q::Driver::WriteByte(const uint8_t byte) const
{
    this->_spi.Write(&byte, sizeof(byte));
}

void N25Q::Driver::WriteAddress(const uint32_t offset) const
{
    uint8_t bytes[] =
    {
        (offset >> 2 * 8) & 0xFF,
        (offset >> 1 * 8) & 0xFF,
        (offset >> 0 * 8) & 0xFF,
    };

    this->_spi.Write(bytes, sizeof(bytes));
}

void N25Q::Driver::WaitForStatus(const Status status, bool state) const
{
    auto expected = state ? status : 0;

    do
    {
        auto flag = this->StatusRegister();

        auto masked = flag & status;

        if (masked == expected)
            return;

    } while (true);
}

void N25Q::Driver::WriteEnable() const
{
    this->Command(Command::WriteEnable, nullptr, 0);
    this->WaitForStatus(Status::WriteEnabled, true);
}

void N25Q::Driver::WriteDisable() const
{
    this->Command(Command::WriteDisable, nullptr, 0);
    this->WaitForStatus(Status::WriteEnabled, false);
}
