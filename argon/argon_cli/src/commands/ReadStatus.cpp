#include "commands/ReadStatus.hpp"

namespace commands
{
    ReadStatus::ReadStatus(GlobalOptions& global, CLI::App& app): 
        _global{global},
        _cmd{app.add_subcommand("read_status", "Read flash status registers")},
        _status{true},
        _status2{false},
        _status3{false},
        _status4{false}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_flag("-1,!-1", _status, "Read status register");
        _cmd->add_flag("-2", _status2, "Read status register 2");
        _cmd->add_flag("-3", _status3, "Read status register 3");
        _cmd->add_flag("-4", _status4, "Read status register 4");
    }

    void ReadStatus::Execute()
    {
        printf("Read status: %c\n", _status ? 'Y' : 'N');
        printf("Read status2: %c\n", _status2 ? 'Y' : 'N');
        printf("Read status3: %c\n", _status3 ? 'Y' : 'N');
        printf("Read status4: %c\n", _status4 ? 'Y' : 'N');
        // TODO: read status
    }
}