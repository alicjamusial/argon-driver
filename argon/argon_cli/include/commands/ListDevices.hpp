#pragma once
#include "CLI/CLI.hpp"

namespace commands
{
    class ListDevices
    {
    public:
        ListDevices(CLI::App& app);

    private:
        void Execute();

        CLI::App* _cmd;
    };
}