#include "commands/Erase.hpp"
#include <optional>

namespace commands
{
    EraseChip::EraseChip(GlobalOptions& global, CLI::App& app) :
        _global{global}, _cmd{app.add_subcommand("erase_chip", "Erase chip")}
    {
        _cmd->callback([this]() { Execute(); });
    }

    void EraseChip::Execute()
    {
        auto spi = _global.ConnectToFlash();
        flash::FlashDriver device{spi};

        std::cout << "Erasing chip..." << std::endl;
        device.EraseChip();
        std::cout << "Done." << std::endl;
    }

    EraseRange::EraseRange(GlobalOptions& global, CLI::App& app) :
        _global{global}, _cmd{app.add_subcommand("erase_range", "Erase sectors in range")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-s,--start", _start, "Erase range: start offset")
            ->required()
            ->transform(CLI::AsSizeValue(false));
        _cmd->add_option("-e,--end", _end, "Erase range: end offset")->required()->transform(CLI::AsSizeValue(false));
    }

    static std::optional<flash::SectorType>
        FindSector(const flash::FlashInformation& flashInfo, std::uint32_t eraseStart, std::uint32_t maxSize)
    {
        std::optional<flash::SectorType> result = std::nullopt;

        for(const auto& sector: flashInfo.SectorTypes)
        {
            auto aligned = (eraseStart % sector.Size) == 0;
            if(!aligned)
            {
                continue;
            }

            if(sector.Size > maxSize)
            {
                continue;
            }

            if(!result.has_value() || result->Size < sector.Size)
            {
                result = sector;
            }
        }

        return result;
    }

    void EraseRange::Execute()
    {
        printf("Erase range\n");
        printf("Start: 0x%06X (%d)\n", _start, _start);
        printf("End: 0x%06X (%d)\n", _end, _end);

        auto spi = _global.ConnectToFlash();
        flash::FlashDriver device{spi};

        auto flashInfo = device.ReadFlashInformation();
        if(!flashInfo)
        {
            printf("Error! Unable to retrieve flash information\n");
            return;
        }

        auto currentOffset = _start;

        while(currentOffset < _end)
        {
            auto sector = FindSector(*flashInfo, currentOffset, _end - currentOffset);
            if(!sector)
            {
                printf("Error! Unable to find sector that can be erase at offset 0x%06X\n", currentOffset);
                break;
            }
            printf(
                "Erasing at 0x%06X using sector size %d bytes (opcode 0x%02X)\n",
                currentOffset,
                sector->Size,
                sector->EraseOpcode);

            device.EraseSector(*sector, currentOffset);

            currentOffset += sector->Size;
        }
    }

    EraseSector::EraseSector(GlobalOptions& global, CLI::App& app) :
        _global{global}, _cmd{app.add_subcommand("erase_sector", "Erase single sector")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-o,--offset", _offset, "Address of sector to erase")
            ->required()
            ->transform(CLI::AsSizeValue(false));
    }

    void EraseSector::Execute()
    {
        printf("Erase sector\n");
        printf("Offset: 0x%06X (%d)\n", _offset, _offset);
        // TODO: erase sector
    }
}