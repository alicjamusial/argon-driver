#include <iostream>

#include "argon/FlashController.hpp"
#include <charconv>
#include <ftd2xx.h>
#include <map>
#include <numeric>
#include <vector>

std::uint32_t listDevices()
{
    DWORD devicesCount = 0;

    FT_CreateDeviceInfoList(&devicesCount);

    std::vector<FT_DEVICE_LIST_INFO_NODE> devices;
    devices.reserve(devicesCount);
    auto r = FT_GetDeviceInfoList(devices.data(), &devicesCount);

    std::cout << "Devices found: " << devicesCount << std::endl;

    for(auto i = 0; i < devicesCount; i++)
    {
        std::cout << "Device " << i << ":" << devices[i].Description << "\t"
                  << devices[i].SerialNumber << std::endl;
    }

    return devicesCount;
}

std::uint32_t listChannels()
{
    uint32 channels;
    SPI_GetNumChannels(&channels);
    printf("\nAvailable SPI channels found: %d\n", channels);

    for(auto i = 0; i < channels; i++)
    {
        FT_DEVICE_LIST_INFO_NODE info;
        SPI_GetChannelInfo(i, &info);

        std::cout << i << ": " << info.Description << "\t" << info.SerialNumber << std::endl;
    }
    return channels;
}

std::string DefineCommandsMapping(std::map<std::string, void (flash::FlashController::*)()>& commandsMapping)
{
    commandsMapping["read_id"] = &flash::FlashController::ReadId;
    commandsMapping["read_all"] = &flash::FlashController::ReadAllMemory;
    commandsMapping["read_flag_status"] = &flash::FlashController::ReadFlagStatus;
    commandsMapping["read_rems"] = &flash::FlashController::ReadRems;
    commandsMapping["status_register"] = &flash::FlashController::ReadStatusRegister;
    commandsMapping["status_register2"] = &flash::FlashController::ReadStatusRegister2;
    commandsMapping["status_register3"] = &flash::FlashController::ReadStatusRegister3;
    commandsMapping["status_register4"] = &flash::FlashController::ReadStatusRegister4;
    commandsMapping["write"] = &flash::FlashController::Write;
    commandsMapping["write_from_file"] = &flash::FlashController::WriteFromFile;
    commandsMapping["erase_chip"] = &flash::FlashController::EraseChip;
    commandsMapping["erase_range"] = &flash::FlashController::EraseRange;
    commandsMapping["erase_sector"] = &flash::FlashController::EraseSector;

    std::vector<std::string> commandLabels;
    for(auto const& command: commandsMapping)
    {
        commandLabels.push_back(command.first);
    }

    std::string allLabels = std::accumulate(
        std::begin(commandLabels),
        std::end(commandLabels),
        std::string(),
        [](std::string& ss, std::string& s) { return ss.empty() ? s : ss + " | " + s; });

    return allLabels;
}

int main(int argc, char** argv)
{
    if(argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        std::cout << "Usage: argon.exe (or use argon-cli.exe for CLI experience)" << std::endl;

        return 0;
    }

    Init_libMPSSE();

    if(listDevices() > 0)
    {
        std::uint32_t channels = listChannels();
        std::uint32_t channelNo = 0;
        std::string channelText;

        std::cout << "\nChoose channel: ";
        std::cin >> channelText;
        std::from_chars(channelText.data(), channelText.data() + channelText.size(), channelNo);

        if(channelNo < channels)
        {
            std::cout << "\nUsing channel: " << channelNo << "\n" << std::endl;

            // Initialize SPI
            SPI spi(channelNo, 1 * 1000 * 1000, ChipSelectLine3 | ChipSelectActiveLow | SpiMode0);
            spi.ChipSelect(false);

            // Initalize flash driver and controller
            flash::FlashDriver flash{spi};
            flash::FlashController flashController{flash};

            std::map<std::string, void (flash::FlashController::*)()> commandsMapping;
            auto allLabels = DefineCommandsMapping(commandsMapping);

            flashController.ReadId();

            std::string action;

            while(action != "exit")
            {
                std::cout << "\nAvailable actions: exit | " << allLabels << std::endl;
                std::cout << "Choose action:";
                std::cin >> action;
                std::cout << "\n";

                auto CommandToExecute = commandsMapping.find(action);
                if(CommandToExecute != commandsMapping.end())
                {
                    (flashController.*commandsMapping[action])();
                }
                else if(action != "exit")
                {
                    std::cout << "Wrong command, try again." << std::endl << std::endl;
                }
            }
        }
        else
        {
            std::cout << "\nWrong channel: " << channelNo << ". It has to be a value of detected "
                      << channels << "channels." << std::endl;
        }
    }
    else
    {
        std::cout << "\nNo devices found. Please connect your Argon to the USB.";
    }

    std::cout << "\nExiting.";
    return 0;
}