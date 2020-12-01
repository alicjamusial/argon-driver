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

        void ReadId();
        void ReadAllMemory(const char* fileName);
        void ReadStatus();
        void Write(uint32_t offset, const uint8_t* data, size_t size);
        void WriteSomething();
        void EraseRange(uint32_t start, uint32_t end);
        void EraseChip();

    private:
        flash::FlashDriver _device;
    };
}
