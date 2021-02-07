#include "commands/ReadId.hpp"
#include "flash_controller/DataFormatter.hpp"
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

                flash::DataFormatter::FormatJedecId(id);
            }

            if(_rems)
            {
                std::cout << "Reading REMS..." << std::endl;
                auto rems = device.ReadRems();

                flash::DataFormatter::FormatRems(rems);
            }
        }
        catch(std::exception e)
        {
            return;
        }
    }
}