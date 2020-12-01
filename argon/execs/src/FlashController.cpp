#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "FlashController.hpp"
#include "FlashDriver.hpp"
#include "SPI.hpp"
#include "helpers.hpp"
#include "mpsse/libMPSSE_spi.h"
#include <cstddef>
#include <ftd2xx.h>

namespace flash
{
    FlashController::FlashController(FlashDriver& device) : _device{device}
    {
    }

    void FlashController::ReadId()
    {
        std::cout << "Reading ID" << std::endl;
        auto id = _device.ReadId();

        std::cout
            << "Manufacturer " << std::uppercase << std::hex << "0x" << (int)id.Manufacturer << std::endl
            << "Memory type " << std::uppercase << std::hex << "0x" << (int)id.MemoryType << std::endl
            << "Memory capacity " << std::uppercase << std::hex << "0x" << (int)id.Capacity << std::endl
            << "Data: ";

        for(auto b: id.Data)
        {
            std::cout << std::uppercase << std::hex << (int)b << " ";
        }

        std::cout << std::endl;
    }

    void FlashController::ReadAllMemory(const char* fileName)
    {
        std::cout << "Reading memory to " << fileName << std::endl;

        auto buffer = new std::array<uint8_t, 1_MB>();

        std::ofstream out(fileName, std::ofstream::binary);

        for(uint32_t offset = 0_MB; offset < 8_MB; offset += buffer->size())
        {
            std::cout << "Reading range from " << std::uppercase << std::hex << offset << " to "
                      << std::uppercase << std::hex << (offset + buffer->size() - 1) << std::endl;

            buffer->fill(0xCC);

            _device.ReadMemory(offset, buffer->data(), buffer->size());

            out.write(reinterpret_cast<char*>(buffer->data()), buffer->size());
        }

        delete buffer;
    }

    void FlashController::ReadStatus()
    {
        std::cout << "Reading status" << std::endl;

        uint8_t status = _device.StatusRegister();

        std::cout << "Status register " << std::uppercase << std::hex << (int)status << std::endl;

        uint8_t flag = _device.FlagStatusRegister();

        std::cout << "Flag status register " << std::uppercase << std::hex << (int)flag << std::endl;
    }

    void FlashController::WriteSomething()
    {
        std::cout << "Writing random data" << std::endl;

        std::array<uint8_t, 15> data{};
        for(auto i = 0; i < data.size(); i++)
        {
            data[i] = 255 - i;
        }

        _device.ProgramMemory(1_MB + 512_KB, data.data(), data.size());
    }

    void FlashController::Write(std::uint32_t offset, const std::uint8_t* data, std::size_t size)
    {
        std::cout << "Writing data" << std::endl;

        _device.ProgramMemory(offset, data, size);
    }

    void FlashController::EraseRange(std::uint32_t start, std::uint32_t end)
    {
        std::cout << "Erasing range" << std::endl;

        for(auto sector = start; sector < end; sector += 4_KB)
        {
            std::cout << "0x" << std::hex << std::uppercase << sector << " " << sector + 4_KB << std::endl;
            _device.EraseSubsector(sector);
        }
    }

    void FlashController::EraseChip()
    {
        std::cout << "Erasing chip" << std::endl;

        _device.EraseChip();
    }

}