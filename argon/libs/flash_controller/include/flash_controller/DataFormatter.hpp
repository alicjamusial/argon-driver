#pragma once

#include "flash_controller/FlashDriver.hpp"

namespace flash
{
    class DataFormatter
    {
    public:
        DataFormatter();

        static void FormatJedecId(Id id);
        static void FormatRems(std::array<uint8_t, 16> id);
    };
}
