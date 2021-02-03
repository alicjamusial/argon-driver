#include <cstdio>
#include "commands/ReadId.hpp"

namespace commands
{
    ReadId::ReadId(GlobalOptions& global,CLI::App& app):
        _global{global},
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
        printf("Selected serial %s\n", _global.Serial().c_str());
        printf("Channel: %d\n", static_cast<int>(_global.Channel()));

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