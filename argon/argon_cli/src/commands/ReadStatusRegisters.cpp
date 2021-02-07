#include "commands/ReadStatusRegisters.hpp"
#include "flash_controller/DataFormatter.hpp"

namespace commands
{
    ReadStatusRegisters::ReadStatusRegisters(GlobalOptions& global, CLI::App& app) :
        _global{global},
        _cmd{app.add_subcommand("read_status_registers", "Read flash status registers")},
        _status{true},
        _status2{false},
        _status3{false},
        _status4{false}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_flag("-1,!--no-1", _status, "Read status register");
        _cmd->add_flag("-2", _status2, "Read status register 2");
        _cmd->add_flag("-3", _status3, "Read status register 3");
        _cmd->add_flag("-4", _status4, "Read status register 4");
    }

    void ReadStatusRegisters::Execute()
    {
        auto spi = _global.ConnectToFlash();
        flash::FlashDriver device{spi};

        if(_status)
        {
            std::cout << "> Reading status register 1..." << std::endl;
            flash::DataFormatter::FormatStatusRegister(device.StatusRegister1());
        }
        if(_status2)
        {
            std::cout << "> Reading status register 2..." << std::endl;
            flash::DataFormatter::FormatStatusRegister(device.StatusRegister2());
        }
        if(_status3)
        {
            std::cout << "> Reading status register 3..." << std::endl;
            flash::DataFormatter::FormatStatusRegister(device.StatusRegister3());
        }
        if(_status4)
        {
            std::cout << "> Reading status register 4..." << std::endl;
            flash::DataFormatter::FormatStatusRegister(device.StatusRegister4());
        }
    }
}