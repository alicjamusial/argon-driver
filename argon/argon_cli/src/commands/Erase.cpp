#include "commands/Erase.hpp"

namespace commands
{
    EraseChip::EraseChip(GlobalOptions& global, CLI::App& app):
        _global{global},
        _cmd{app.add_subcommand("erase_chip", "Erase chip")}
    {
        _cmd->callback([this]() { Execute(); });
    }

    void EraseChip::Execute()
    {
        printf("Erase chip\n");
        // TODO: erase chip
    }

    EraseRange::EraseRange(GlobalOptions& global, CLI::App& app):
        _global{global},
        _cmd{app.add_subcommand("erase_range", "Erase sectors in range")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-s,--start", _start, "Erase range: start offset")
            ->required()
            ->transform(CLI::AsSizeValue(false));
        _cmd->add_option("-e,--end", _end, "Erase range: end offset")
            ->required()
            ->transform(CLI::AsSizeValue(false));
    }

    void EraseRange::Execute()
    {
        printf("Erase range\n");
        printf("Start: 0x%06X (%d)\n", _start, _start);
        printf("End: 0x%06X (%d)\n", _end, _end);
        // TODO: erase range
    }

    EraseSector::EraseSector(GlobalOptions& global, CLI::App& app):
        _global{global},
        _cmd{app.add_subcommand("erase_sector", "Erase single sector")}
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