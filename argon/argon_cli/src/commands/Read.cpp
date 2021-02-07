#include "commands/Read.hpp"

namespace commands
{
    Read::Read(GlobalOptions& global, CLI::App& app) :
        _global{global},
        _cmd{app.add_subcommand("read", "Read data from flash")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-s,--start", _start, "Read range: start offset")->required()->transform(CLI::AsSizeValue(false));

        _cmd->add_option("-e,--end", _end, "Read range: end offset")->required()->transform(CLI::AsSizeValue(false));

        _cmd->add_option("-f,--file", _outputFilePath, "Output file path")->required();
    }

    void Read::Execute()
    {
        printf("Read data\n");
        printf("Start: 0x%08X (%d)\n", _start, _start);
        printf("End: 0x%08X (%d)\n", _start, _start);
        printf("Output file: %s\n", _outputFilePath.c_str());
    }
}