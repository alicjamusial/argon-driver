#pragma once
#include "CLI/CLI.hpp"

namespace commands
{
    class ReadId
    {
        public:
            ReadId(CLI::App& app);

        private:
            void Execute();
            CLI::App* _cmd;

            bool _jedec;
            bool _rems;
    };
}