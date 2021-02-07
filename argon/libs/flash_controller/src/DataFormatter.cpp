#include "flash_controller/DataFormatter.hpp"
#include <iomanip>
#include <iostream>

namespace flash
{
    DataFormatter::DataFormatter()
    {
    }

    void DataFormatter::FormatJedecId(Id id)
    {
        std::cout << "> Manufacturer: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)id.Manufacturer << std::endl
                  << "> Memory type: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)id.MemoryType << std::endl
                  << "> Memory capacity: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)id.Capacity << std::endl
                  << "> Data: ";

        for(auto b: id.Data)
        {
            std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << (int)b << " ";
        }

        std::cout << std::endl;
    }

    void DataFormatter::FormatRems(std::array<std::uint8_t, 16> rems)
    {
        for(auto elem: rems)
        {
            std::cout << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                      << (int)elem << " ";
        }

        std::cout << std::endl;
    }

    void DataFormatter::FormatStatusRegister(std::uint8_t status)
    {
        std::cout << "> Status register: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)status << std::endl;
    }

    void DataFormatter::FormatFlagStatusRegister(std::uint8_t status)
    {
        std::cout << "> Status flag register: "
                  << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex
                  << (int)status << std::endl;
    }
}