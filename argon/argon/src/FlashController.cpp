#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "argon/FlashController.hpp"
#include "flash_controller/DataFormatter.hpp"
#include "flash_controller/FlashDriver.hpp"
#include "flash_controller/SPI.hpp"
#include "flash_controller/helpers.hpp"
#include "mpsse/libMPSSE_spi.h"
#include <charconv>
#include <cstddef>
#include <ftd2xx.h>

namespace flash
{
    FlashController::FlashController(FlashDriver& device) : _device{device}
    {
    }

    void FlashController::ReadId()
    {
        std::cout << "> Reading flash ID..." << std::endl;
        auto id = _device.ReadId();

        flash::DataFormatter::FormatJedecId(id);
    }

    void FlashController::ReadRems()
    {
        std::cout << "> Reading REMS..." << std::endl;
        auto rems = _device.ReadRems();

        flash::DataFormatter::FormatRems(rems);
    }

    void FlashController::ReadAllMemory()
    {
        std::string fileName;
        std::cout << "> Choose path to read to (e.g. D:/test.bin): ";
        std::cin >> fileName;
        std::cout << "\n";

        std::cout << "> Reading memory to " << fileName << std::endl;

        std::array<uint8_t, 1_MB> buffer{};

        std::ofstream out(fileName, std::ofstream::binary);

        for(uint32_t offset = 0_MB; offset < 8_MB; offset += buffer.size())
        {
            std::cout << "> Reading range from "
                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << offset << " to "
                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << (offset + buffer.size() - 1) << std::endl;

            buffer.fill(0xCC);

            _device.ReadMemory(offset, buffer.data(), buffer.size());

            out.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
        }
    }

    void FlashController::ReadFlagStatus()
    {
        std::cout << "> Reading flag status..." << std::endl;

        uint8_t flag = _device.FlagStatusRegister();

        std::cout << "> Flag status register: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)flag << std::endl;
    }

    void FlashController::ReadStatusRegister()
    {
        std::cout << "> Reading status register..." << std::endl;
        std::cout << "> Status register: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)_device.StatusRegister1() << std::endl;
    }

    void FlashController::ReadStatusRegister2()
    {
        std::cout << "> Reading status register 2..." << std::endl;
        std::cout << "> Status register 2: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)_device.StatusRegister2() << std::endl;
    }

    void FlashController::ReadStatusRegister3()
    {
        std::cout << "> Reading status register 3..." << std::endl;
        std::cout << "> Status register 3: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)_device.StatusRegister3() << std::endl;
    }

    void FlashController::ReadStatusRegister4()
    {
        std::cout << "> Reading status register 4..." << std::endl;
        std::cout << "> Status register 4: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)_device.StatusRegister4() << std::endl;
    }

    void FlashController::Write()
    {
        std::cout << "> Writing data..." << std::endl;
        std::string offsetText;
        std::string locationText;

        std::cout << "> Choose offset: ";
        std::cin >> offsetText;
        std::cout << "> Choose data to write: ";
        std::cin >> locationText;
        std::cout << std::endl;

        std::uint32_t offset = 0;
        std::from_chars(offsetText.data(), offsetText.data() + offsetText.size(), offset);

        _device.ProgramMemory(
            offset, reinterpret_cast<const uint8_t*>(locationText.data()), locationText.size());
    }

    void FlashController::WriteFromFile()
    {
        std::cout << "> Writing from file... Not yet implemented" << std::endl;
        // TODO: Write from file
    }

    void FlashController::EraseRange()
    {
        std::cout << "> Erasing range... Not yet implemented" << std::endl;

        // TODO: Erase range

        //        for(auto sector = start; sector < end; sector += 4_KB)
        //        {
        //            std::cout << "0x"
        //                      << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
        //                      << sector << " " << sector + 4_KB << std::endl;
        //            _device.EraseSubsector(sector);
        //        }
    }

    void FlashController::EraseChip()
    {
        std::cout << "> Erasing chip..." << std::endl;
        _device.EraseChip();
    }

    void FlashController::EraseSector()
    {
        std::cout << "> Erasing sector... Not yet implemented" << std::endl;
        // TODO: Erase sector
    }

}