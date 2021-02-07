#include <iostream>

#include "flash_controller/FlashDriver.hpp"

namespace flash
{
    FlashDriver::FlashDriver(SPI& spi) : _spi{spi}
    {
    }

    void FlashDriver::Command(CommandType commandCode, std::uint8_t* response, std::size_t responseSize) const
    {
        SPISelectSlave select(this->_spi);

        this->CommandNoSelect(commandCode, response, responseSize);
    }

    void FlashDriver::CommandNoSelect(const CommandType commandCode, std::uint8_t* response, std::size_t responseSize) const
    {
        this->WriteCommand(commandCode);
        this->_spi.Read(response, responseSize);
    }

    Id FlashDriver::ReadId() const
    {
        std::uint8_t response[20] = {0};

        this->Command(CommandType::ReadId, response, sizeof(response));

        std::array<std::uint8_t, 17> d{};
        for(auto i = 3; i < 20; i++)
        {
            d[i - 3] = response[i];
        }

        return Id(response[0], response[1], response[2], d);
    }

    std::array<std::uint8_t, 16> FlashDriver::ReadRems() const
    {
        {
            SPISelectSlave select(this->_spi);

            this->WriteCommand(CommandType::ReadRems);
            this->WriteAddress(0x00);

            std::array<std::uint8_t, 16> response;
            this->_spi.Read(reinterpret_cast<uint8_t*>(&response), 16);

            return response;
        }
    }

    Status FlashDriver::ParseStatusRegister() const
    {
        std::uint8_t status;
        this->Command(CommandType::ReadStatusRegister1, &status, sizeof(status));

        auto parsedStatus = static_cast<Status>(status);

        return parsedStatus;
    }

    std::uint8_t FlashDriver::StatusRegister1() const
    {
        std::uint8_t status{0};
        this->Command(CommandType::ReadStatusRegister1, &status, sizeof(status));
        return status;
    }

    std::uint8_t FlashDriver::StatusRegister2() const
    {
        std::uint8_t status{0};
        this->Command(CommandType::ReadStatusRegister2, &status, sizeof(status));
        return status;
    }

    std::uint8_t FlashDriver::StatusRegister3() const
    {
        std::uint8_t status{0};
        this->Command(CommandType::ReadStatusRegister3, &status, sizeof(status));
        return status;
    }

    std::uint8_t FlashDriver::StatusRegister4() const
    {
        std::uint8_t status{0};
        this->Command(CommandType::ReadStatusRegister4, &status, sizeof(status));
        return status;
    }

    std::uint8_t FlashDriver::FlagStatusRegister() const
    {
        std::uint8_t status;
        this->Command(CommandType::ReadFlagStatusRegister, &status, sizeof(status));

        return status;
    }

    void FlashDriver::WaitBusy() const
    {
        do
        {
            Sleep(100);
            std::cout << "> Operation in progress..." << std::endl;
            std::uint8_t status = 0;

            this->CommandNoSelect(CommandType::ReadStatusRegister1, &status, sizeof(status));

            if((static_cast<Status>(status) & Status::WriteInProgress) == 0)
            {
                break;
            }
        } while(true);
    }

    void FlashDriver::ReadMemory(std::uint32_t offset, std::uint8_t* buffer, std::size_t size) const
    {
        SPISelectSlave select(this->_spi);

        this->WriteCommand(CommandType::ReadMemory);
        this->WriteAddress(offset);

        this->_spi.Read(buffer, size);
    }

    void FlashDriver::ProgramMemory(std::uint32_t offset, const std::uint8_t* data, std::size_t size) const
    {
        this->WriteEnable();

        {
            SPISelectSlave select(this->_spi);

            this->WriteCommand(CommandType::ProgramMemory);
            this->WriteAddress(offset);
            this->_spi.Write(data, size);
        }
        {
            SPISelectSlave select(this->_spi);

            this->WaitBusy();
        }

        this->WriteDisable();
    }

    void FlashDriver::EraseSubsector(std::uint32_t offset) const
    {
        this->WriteEnable();

        {
            SPISelectSlave select(this->_spi);

            this->WriteCommand(CommandType::EraseSubsector);
            this->WriteAddress(offset);
        }

        {
            SPISelectSlave select(this->_spi);

            this->WaitBusy();
        }

        this->WriteDisable();
    }

    void FlashDriver::EraseChip() const
    {
        this->WriteEnable();

        {
            SPISelectSlave select(this->_spi);
            this->WriteCommand(CommandType::EraseChip);
        }

        {
            SPISelectSlave select(this->_spi);

            this->WaitBusy();
        }

        this->WriteDisable();
    }

    void FlashDriver::WriteCommand(CommandType byte) const
    {
        auto byteToWrite = static_cast<std::uint8_t const>(byte);
        this->_spi.Write(&byteToWrite, sizeof(byteToWrite));
    }

    void FlashDriver::WriteAddress(const uint32_t offset) const
    {
        std::uint8_t bytes[] = {
            static_cast<uint8_t>((offset >> 2 * 8) & 0xFF),
            static_cast<uint8_t>((offset >> 1 * 8) & 0xFF),
            static_cast<uint8_t>((offset >> 0 * 8) & 0xFF),
        };

        this->_spi.Write(bytes, sizeof(bytes));
    }

    void FlashDriver::WaitForStatus(const Status status, bool state) const
    {
        auto expected = state ? status : 0;

        do
        {
            auto flag = this->ParseStatusRegister();

            auto masked = flag & status;

            if(masked == expected)
            {
                return;
            }

        } while(true);
    }

    void FlashDriver::WriteEnable() const
    {
        this->Command(CommandType::WriteEnable, nullptr, 0);
        this->WaitForStatus(Status::WriteEnabled, true);
    }

    void FlashDriver::WriteDisable() const
    {
        this->Command(CommandType::WriteDisable, nullptr, 0);
        this->WaitForStatus(Status::WriteEnabled, false);
    }
}