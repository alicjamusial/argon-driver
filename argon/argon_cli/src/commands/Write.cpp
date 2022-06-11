#include "commands/Write.hpp"
#include "flash_controller/helpers.hpp"

namespace commands
{
    Write::Write(GlobalOptions& global, CLI::App& app) :
        _global{global},
        _cmd{app.add_subcommand("write", "Write data to flash")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-s,--start", _start, "Write range: start offset")
            ->required()
            ->transform(CLI::AsSizeValue(false));

        _cmd->add_option("-f,--file", _inputFilePath, "Input file path")->required()->check(CLI::ExistingFile);
    }

    void Write::Execute()
    {
        auto spi = _global.ConnectToFlash();
        flash::FlashDriver device{spi};

        std::ifstream in(_inputFilePath, std::ifstream::binary | std::ifstream::ate);
        auto size = static_cast<uint32_t>(in.tellg());
        in.seekg(0);

        std::array<std::uint8_t, 1_MB> buffer{};

        std::cout << "> Writing memory from 0x" << std::hex << _start << " to 0x" << std::hex
                  << (_start + size) << " from file " << _inputFilePath.c_str() << std::endl;

        for(uint32_t offset = 0; offset < size;)
        {
            auto write_size = std::min(static_cast<std::uint32_t>(buffer.size()), size - offset);

            in.read(reinterpret_cast<char*>(buffer.data()), write_size);

            std::cout << "> Writing range from "
                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << (_start + offset) << " to "
                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << (_start + offset + write_size - 1) << std::endl;

            device.ProgramMemory(_start + offset, reinterpret_cast<const uint8_t*>(buffer.data()), write_size);

            offset += write_size;
        }
    }
}