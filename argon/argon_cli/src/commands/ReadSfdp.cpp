#include "commands/ReadSfdp.hpp"

namespace commands
{
    ReadSfdp::ReadSfdp(GlobalOptions& global, CLI::App& app) :
        _cmd{app.add_subcommand("read_info", "Reads basic flash information using SFDP")}, _global{global}
    {
        _cmd->callback([this]() { Execute(); });
    }

    void ReadSfdp::Execute()
    {
        printf("Reading SFDP\n");

        auto spi = _global.ConnectToFlash();
        flash::FlashDriver flash{spi};

        auto parameters = flash.ReadSFDP();
        printf("Found %zu parameters:\n", parameters.size());

        for(auto& p: parameters)
        {
            printf("\tID: 0x%04X Offset: 0x%06X Size: %d \n", p.ParameterId, p.Offset, p.Size);
        }

        auto info = flash.ReadFlashInformation();

        if(!info.has_value())
        {
            printf("Unable to retrieve basic flash information (SFDP not supported?)\n");
            return;
        }

        printf("Density: %zu bytes (%zu megabytes)\n", info->SizeInBytes, info->SizeInBytes / 1024 / 1024);

        for(auto& s: info->SectorTypes)
        {
            printf("Sector Type %d: size %d opcode: 0x%02X\n", s.Index, s.Size, s.EraseOpcode);
        }
    }
}