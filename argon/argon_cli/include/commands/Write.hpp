#pragma once
#include "GlobalOptions.hpp"
#include "CLI/CLI.hpp"

namespace commands
{
    class Write
    {
        public:
            Write(GlobalOptions& global, CLI::App& app);

        private:
            void Execute();

            GlobalOptions& _global;
            CLI::App* _cmd;

            std::uint32_t _start;
            std::string _inputFilePath;
    };
}