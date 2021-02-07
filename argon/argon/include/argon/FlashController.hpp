#pragma once

#include "flash_controller/FlashDriver.hpp"
#include <algorithm>
#include <array>

namespace flash
{
    class FlashController
    {
    public:
        FlashController(FlashDriver& device);

        void ReadId();
        void ReadAllMemory();
        void ReadFlagStatus();
        void ReadRems();
        void ReadStatusRegister();
        void ReadStatusRegister2();
        void ReadStatusRegister3();
        void ReadStatusRegister4();
        void Write();
        void WriteFromFile();
        void EraseRange();
        void EraseChip();
        void EraseSector();

    private:
        flash::FlashDriver _device;
    };
}
