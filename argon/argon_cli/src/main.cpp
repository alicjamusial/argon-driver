#include "CLI/CLI.hpp"
#include "GlobalOptions.hpp"
#include "commands/Erase.hpp"
#include "commands/ListDevices.hpp"
#include "commands/RawSPI.hpp"
#include "commands/Read.hpp"
#include "commands/ReadId.hpp"
#include "commands/ReadSfdp.hpp"
#include "commands/ReadStatusRegisters.hpp"
#include "commands/Write.hpp"
#include <cstdio>

int main(int argc, char** argv)
{
    CLI::App app{"Argon CLI"};
    GlobalOptions global{app};

    commands::ListDevices listDevices{app};
    commands::ReadId readId{global, app};
    commands::ReadSfdp readSfdp{global, app};

    commands::EraseChip eraseChip{global, app};
    commands::EraseRange eraseRange{global, app};
    commands::EraseSector eraseSector{global, app};

    commands::RawSPI rawSPI{global, app};

    commands::Read read{global, app};

    commands::ReadStatusRegisters readStatusRegisters{global, app};

    commands::Write write{global, app};

    app.require_subcommand(1);

    CLI11_PARSE(app, argc, argv);
    return 0;
}