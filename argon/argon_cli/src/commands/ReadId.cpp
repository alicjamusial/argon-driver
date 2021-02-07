#include "commands/ReadId.hpp"
#include <cstdio>

namespace commands
{
    ReadId::ReadId(GlobalOptions& global, CLI::App& app) :
        _global{global},
        _cmd{app.add_subcommand("read_id", "Read flash ID")},
        _jedec{true},
        _rems{false}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_flag("--jedec,!--no-jedec", _jedec, "Read JEDEC ID");
        _cmd->add_flag("--rems", _rems, "Read REMS");
    }

    void ReadId::Execute()
    {
        try
        {
            auto spi = _global.ConnectToFlash();
            flash::FlashDriver device{spi};

            if(_jedec)
            {
                std::cout << "Reading JEDEC ID..." << std::endl;
                auto id = device.ReadId();

                std::cout << "> Manufacturer: "
                          << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                          << "0x" << (int)id.Manufacturer << std::endl
                          << "> Memory type: "
                          << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                          << "0x" << (int)id.MemoryType << std::endl
                          << "> Memory capacity: "
                          << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                          << "0x" << (int)id.Capacity << std::endl
                          << "> Data: ";

                for(auto b: id.Data)
                {
                    std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                              << (int)b << " ";
                }

                std::cout << std::endl;
            }

            if(_rems)
            {
                std::cout << "Reading REMS..." << std::endl;
                device.ReadRems();
            }
        }
        catch(std::exception e)
        {
            return;
        }
    }
}