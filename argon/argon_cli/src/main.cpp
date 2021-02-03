#include <cstdio>
#include "CLI/CLI.hpp"

int main(int argc, char** argv)
{
    CLI::App app{"Argon"};


    auto readIdCmd = app.add_subcommand("read_id", "Read flash JEDEC ID");
    auto eraseCmd = app.add_subcommand("erase", "Erase flash sections");
    auto readCmd = app.add_subcommand("read", "Read data from flash");

    std::uint32_t startAddress;
    eraseCmd->add_option("--start", startAddress, "Address of first sector to erase")->required();
    readCmd->add_option("--start", startAddress, "Address where reading starts")->required();
    std::string outputFile;
    readCmd->add_option("--output", outputFile, "Output file")->required();

    readIdCmd->callback([]() {
        printf("Reading id\n");
    });

    eraseCmd->callback([&]() {
        printf("Erasing\n");
        printf("Start: 0x%04X\n", startAddress);
    });

    readCmd->callback([&](){
        printf("Reading\n");
        printf("Start: 0x%04X\n", startAddress);
        printf("Output file: %s\n", outputFile.c_str());
    });

    app.require_subcommand(1);

    CLI11_PARSE(app, argc, argv);
    return 0;
}