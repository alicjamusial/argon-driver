#pragma once
#include "CLI/CLI.hpp"
#include "GlobalOptions.hpp"
#include "flash_controller/FlashDriver.hpp"

namespace commands
{
    class Read
    {
    public:
        Read(GlobalOptions& global, CLI::App& app);

    private:
        void Execute();

        GlobalOptions& _global;
        CLI::App* _cmd;

        std::uint32_t _start;
        std::uint32_t _end;
        std::string _outputFilePath;
    };
}