#include "commands/RawSPI.hpp"

namespace commands
{
    RawSPI::RawSPI(GlobalOptions& global, CLI::App& app): 
        _global{global},
        _cmd{app.add_subcommand("raw_spi", "Perform raw SPI transcation")}
    {
        _cmd->callback([this]() { Execute(); });

        _cmd->add_option("-t,--tx", _tx)->required()->description("Bytes to send (separated by spaces)");
        _cmd->add_option("-r,--rx", _rxCount, "Number of bytes to receive as response")->required();
    }

    void RawSPI::Execute()
    {
        printf("Raw SPI\n");
        printf("TX bytes: ");
        for(auto b: _tx) {
            printf("0x%02X ", b);
        }
        printf("\n");
        printf("Receive count: %d\n", _rxCount);
        // TODO: perform raw SPI
    }
}