#pragma once
#include "CLI/CLI.hpp"
#include "GlobalOptions.hpp"

namespace commands
{
    class ReadSfdp
    {
    public:
        ReadSfdp(GlobalOptions& global, CLI::App& app);

    private:
        void Execute();

        GlobalOptions& _global;
        CLI::App* _cmd;
    };
}
