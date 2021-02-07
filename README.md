# ARGON - SPI NOR Flash programmer & reader

<img align="center" width="100" height="100" src="http://alicja.space/argon-logo.png"> 

### ◾ About the project

Argon project consists of two parts: 
- hardware: [Argon board based on FT2232H microcontroller](https://github.com/alicjamusial/argon/)
- software included in this repo.

This driver can be used both with Argon board or any FTDI-based board with SPI flash attached.

Tested with N25Q, S25FL512S and Winbond W25Q flashes.

**Remember**: different flashes can have various command addresses. If a command does not work on your flash device, there can be two reasons: it can have different address on this particular flash or your flash doesn't support this particular command at all. When in doubt, look into datasheets.

### ◾ Running software
There are two execs included in [releases section]():
- `argon.exe` - manual command line interface (handy on flash debugging)
- `argon_cli.exe` - CLI for automation (run `argon_cli.exe --help` for usage hints). Example usage: `argon_cli.exe list`, `argon_cli.exe --serial FT613ZHMA read_id`

### ◾ Building software
#### Prerequisites
- MinGW
- FTDI drivers
- `local-example.cmake` - filled with proper FTDI drivers path and copied to folder `.local` with a name `local.cmake`
#### Building
- in build directory: `cmake.exe -DCMAKE_BUILD_TYPE=Debug -DLOCAL_CFG=[project path]/.local/local.cmake [project path]`
- in build directory: `make`

### Contributors
<a href="https://github.com/alicjamusial/argon-driver/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=alicjamusial/argon-driver" />
</a>

### ◾ TODO
Commands:
- `erase_range`
- `erase_sector`
- `write_from_file`
- `custom command`

### ◾ Known issues
No known issues. If you find any - PRs are welcomed :)