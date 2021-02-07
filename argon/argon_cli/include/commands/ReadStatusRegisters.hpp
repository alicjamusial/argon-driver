#pragma once
#include "CLI/CLI.hpp"
#include "GlobalOptions.hpp"

namespace commands
{
    class ReadStatusRegisters
    {
    public:
        ReadStatusRegisters(GlobalOptions& global, CLI::App& app);

    private:
        void Execute();

        GlobalOptions& _global;
        CLI::App* _cmd;

        bool _status;
        bool _status2;
        bool _status3;
        bool _status4;
    };
}