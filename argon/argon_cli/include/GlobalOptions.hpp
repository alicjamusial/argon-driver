#pragma once
#include "CLI/CLI.hpp"

enum class ArgonChannel
{
    A,
    B
};

class GlobalOptions
{
    public:
        GlobalOptions(const GlobalOptions&) = delete;

        GlobalOptions(CLI::App& app);

        const std::string& Serial() const;
        const ArgonChannel& Channel() const;

    private:
        std::string _serial;
        ArgonChannel _channel;
};

inline const std::string& GlobalOptions::Serial() const
{
    return _serial;
}

inline const ArgonChannel& GlobalOptions::Channel() const
{
    return _channel;
}
