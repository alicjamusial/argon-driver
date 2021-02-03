#pragma once
#include "GlobalOptions.hpp"
#include "CLI/CLI.hpp"

namespace commands
{
    class EraseChip
    {
        public:
            EraseChip(GlobalOptions& global, CLI::App& app);

        private:
            void Execute();

            GlobalOptions& _global;
            CLI::App* _cmd;
    };

    class EraseRange
    {
        public:
            EraseRange(GlobalOptions& global, CLI::App& app);

        private:
            void Execute();

            GlobalOptions& _global;
            CLI::App* _cmd;

            std::uint32_t _start;
            std::uint32_t _end;
    };

    class EraseSector
    {
        public:
            EraseSector(GlobalOptions& global, CLI::App& app);

        private:
            void Execute();

            GlobalOptions& _global;
            CLI::App* _cmd;

            std::uint32_t _offset;
    };
}