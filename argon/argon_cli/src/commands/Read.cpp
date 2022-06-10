#include "commands/Read.hpp"
#include "flash_controller/helpers.hpp"

namespace commands
{
    Read::Read(GlobalOptions& global, CLI::App& app) :
        _global{global},
        _cmd{app.add_subcommand("read", "Read data from flash")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-s,--start", _start, "Read range: start offset")->required()->transform(CLI::AsSizeValue(false));

        _cmd->add_option("-e,--end", _end, "Read range: end offset")->required()->transform(CLI::AsSizeValue(false));

        _cmd->add_option("-f,--file", _outputFilePath, "Output file path")->required();
    }

    void Read::Execute()
    {
        auto spi = _global.ConnectToFlash();
        flash::FlashDriver device{spi};

        std::cout << "> Reading memory from 0x" << std::hex << _start << " to 0x" << std::hex
                  << _end << " to file " << _outputFilePath.c_str() << std::endl;

        std::array<std::uint8_t, 1_MB> buffer{};

        std::ofstream out(_outputFilePath.c_str(), std::ofstream::binary);

        for(uint32_t offset = _start; offset < _end;)
        {
            auto read_size = std::min(static_cast<std::uint32_t>(buffer.size()), _end - offset);
            std::cout << "> Reading range from "
                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << offset << " to "
                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << (offset + read_size - 1) << std::endl;

            buffer.fill(0xCC);

            device.ReadMemory(offset, buffer.data(), read_size);

            out.write(reinterpret_cast<char*>(buffer.data()), read_size);
            offset += read_size;
        }
    }
}