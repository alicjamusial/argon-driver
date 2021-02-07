#pragma once
#include "CLI/CLI.hpp"
#include "GlobalOptions.hpp"
#include "flash_controller/FlashDriver.hpp"

namespace commands
{
    class ReadId
    {
    public:
        ReadId(GlobalOptions& global, CLI::App& app);

    private:
        void Execute();

        GlobalOptions& _global;
        CLI::App* _cmd;

        bool _jedec;
        bool _rems;
    };
}