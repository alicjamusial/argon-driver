#pragma once
#include "GlobalOptions.hpp"
#include "CLI/CLI.hpp"
#include <vector>

namespace commands
{
    class RawSPI
    {
        public:
            RawSPI(GlobalOptions& global, CLI::App& app);

        private:
            void Execute();

            GlobalOptions& _global;
            CLI::App* _cmd;

            std::vector<std::uint8_t> _tx;
            std::uint32_t _rxCount;
    };
}