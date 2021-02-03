#include <cstdio>
#include "CLI/CLI.hpp"
#include "GlobalOptions.hpp"
#include "commands/ReadId.hpp"
#include "commands/ListDevices.hpp"

int main(int argc, char** argv)
{
    CLI::App app{"Argon"};

    GlobalOptions global{app};

    commands::ListDevices listDevices{app};
    commands::ReadId readId{global, app};

    app.require_subcommand(1);

    CLI11_PARSE(app, argc, argv);
    return 0;
}