#include "commands/ListDevices.hpp"

namespace commands
{
    ListDevices::ListDevices(CLI::App& app):
        _cmd{app.add_subcommand("list", "List connected FTDI devices")}
    {
        _cmd->callback([this](){ Execute(); });
    }

    void ListDevices::Execute()
    {
        printf("Listing devices\n");
        // TODO: list devices
    }
}