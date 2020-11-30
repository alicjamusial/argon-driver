#pragma once

#include "FlashDriver.hpp"
#include <algorithm>
#include <array>

namespace flash
{
    class FlashController
    {
    public:
        FlashController(FlashDriver& device);

        void ReadId(flash::FlashDriver& device);
        void ReadAllMemory(FlashDriver& device, const char* fileName);
        void ReadStatus(FlashDriver& device);
        void WriteSomething(FlashDriver& device);
        void EraseRange(FlashDriver& device, uint32_t start, uint32_t end);

    private:
        flash::FlashDriver _device;
    };
}
