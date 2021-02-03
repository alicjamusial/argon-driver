#include <cstdio>
#include "commands/ReadId.hpp"

namespace commands
{
    ReadId::ReadId(CLI::App& app):
        _cmd{app.add_subcommand("read_id", "Read flash ID")},
        _jedec{true},
        _rems{false}
    {
        _cmd->callback([this](){ Execute(); });

        _cmd->add_flag("--jedec,!--no-jedec", _jedec, "Read JEDEC ID");
        _cmd->add_flag("--rems", _rems, "Read REMS");
    }

    void ReadId::Execute()
    {
        if(_jedec)
        {
            printf("Reading JEDEC ID\n");
            // TODO: read id
        }

        if(_rems)
        {
            printf("Reading REMS\n");
            // TODO: read rems
        }
    }
}