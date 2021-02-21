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

    std::vector<FlashParameter> FlashDriver::ReadSFDP()
    {
        SPISelectSlave select(this->_spi);
        std::uint8_t command[] = {
            0x5A,
            0x0,
            0x0,
            0x0,
            0xFF,
        };
        std::array<std::uint8_t, 8> response{};
        this->_spi.Write(command, sizeof(command));
        this->_spi.Read(response.data(), response.size());

        bool isValidResponse = true;
        isValidResponse &= response[0] == 'S';
        isValidResponse &= response[1] == 'F';
        isValidResponse &= response[2] == 'D';
        isValidResponse &= response[3] == 'P';

        if(!isValidResponse)
        {
            return {};
        }

        std::uint8_t numberOfParameters = response[6] + 1;

        std::vector<FlashParameter> parameters;
        parameters.reserve(numberOfParameters);

        for(int i = 0; i < numberOfParameters; i++)
        {
            std::array<std::uint8_t, 8> header{};

            this->_spi.Read(header.data(), header.size());

            FlashParameter parameter{};
            parameter.Offset = (header[4] << 0) | (header[5] << 8) | (header[6] << 16);
            parameter.Size = header[3] * 4;
            parameter.ParameterId = (header[0] << 0) | (header[7] << 8);

            parameters.push_back(parameter);
        }

        return parameters;
    }

    std::vector<std::uint8_t> FlashDriver::ReadParameter(const FlashParameter& parameter)
    {
        std::vector<std::uint8_t> response(parameter.Size, std::uint8_t{0});

        SPISelectSlave select(this->_spi);
        std::uint8_t command[] = {
            0x5A,
            static_cast<std::uint8_t>((parameter.Offset >> 16) & 0xFF),
            static_cast<std::uint8_t>((parameter.Offset >> 8) & 0xFF),
            static_cast<std::uint8_t>((parameter.Offset >> 0) & 0xFF),
            0xFF,
        };
        this->_spi.Write(command, sizeof(command));
        this->_spi.Read(response.data(), response.size());

        return response;
    }

    std::optional<FlashInformation> FlashDriver::ReadFlashInformation()
    {
        auto parameters = ReadSFDP();
        if(parameters.empty())
        {
            return std::nullopt;
        }

        auto baseInfo = ReadParameter(parameters[0]);

        FlashInformation result;

        auto density =
            (baseInfo[4] << 0) | (baseInfo[5] << 8) | (baseInfo[6] << 16) | (baseInfo[7] << 24);
        density++;

        result.SizeInBytes = density / 8;

        for(int i = 0; i < 4; i++)
        {
            auto base = 7 * 4 + i * 2;

            auto size = 1 << baseInfo[base + 0];
            auto opcode = baseInfo[base + 1];

            if(size == 1)
            {
                continue;
            }

            SectorType sector{};
            sector.Index = i + 1;
            sector.Size = size;
            sector.EraseOpcode = opcode;

            result.SectorTypes.push_back(sector);
        }

        return result;
    }

    void FlashDriver::EraseSector(const SectorType& sectorType, std::uint32_t offset)
    {
        this->WriteEnable();

        {
            SPISelectSlave select(this->_spi);

            this->WriteCommand(CommandType{sectorType.EraseOpcode});
            this->WriteAddress(offset);
        }

        {
            SPISelectSlave select(this->_spi);

            this->WaitBusy();
        }

        this->WriteDisable();
    }
}