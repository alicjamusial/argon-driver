#include "GlobalOptions.hpp"
#include <map>

static std::map<std::string, ArgonChannel> ChannelMap{{"A", ArgonChannel::A}, {"B", ArgonChannel::B}};

GlobalOptions::GlobalOptions(CLI::App& app) : _serial{""}, _channel{ArgonChannel::A}
{
    app.add_option("--serial", _serial, "Argon serial number");
    app.add_option("--channel", _channel, "Argon channel")
        ->transform(CLI::CheckedTransformer{ChannelMap, CLI::ignore_case});
}