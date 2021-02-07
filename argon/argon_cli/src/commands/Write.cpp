#include "commands/Write.hpp"

namespace commands
{
    Write::Write(GlobalOptions& global, CLI::App& app) :
        _global{global},
        _cmd{app.add_subcommand("write", "Write data to flash")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-s,--start", _start, "Write range: start offset")
            ->required()
            ->transform(CLI::AsSizeValue(false));

        _cmd->add_option("-f,--file", _inputFilePath, "Input file path")->required()->check(CLI::ExistingFile);
    }

    void Write::Execute()
    {
        printf("Write data\n");
        printf("Start: 0x%08X (%d)\n", _start, _start);
        printf("Input file: %s\n", _inputFilePath.c_str());
        // TODO: write data from file
    }
}