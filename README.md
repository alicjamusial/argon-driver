# ARGON - SPI NOR Flash programmer & reader

### ◾ About the project

<img align="left" width="100" height="100" src="http://alicja.space/argon-logo.png"> 
Argon project consists of two parts: 
- hardware: [Argon board based on FT2232H microcontroller](https://github.com/alicjamusial/argon/)
- software included in this repo.

This driver can be used both with Argon board or any FTDI-based board with SPI flash attached.

Tested with N25Q, S25FL512S and Winbond W25Q flashes.

**Remember**: different flashes can have various command addresses. If a command does not work on your flash device, there can be two reasons: it can have different address on this particular flash or your flash doesn't support this particular command at all. When in doubt, look into datasheets.

### ◾ Running software
There are two versions of software included in [realases section]():
- `argon.exe`
- `argon_cli.exe`

### ◾ Building software
#### Prerequisites
- MinGW
- FTDI drivers
- `local-example.cmake` filled and copied to folder `.local` with a name `local.cmake`
#### Building
TBD

### ◾ TODO
Commands:
- `erase_range`
- `erase_sector`
- `write_from_file`
- `custom command`

### ◾ Known issues
No known issues. If you find any - PRs are welcomed :)