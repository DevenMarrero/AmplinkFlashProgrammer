# FT4232 Amplink Programmer
This project provides drivers and programming features to utilize an AmPLink FTDI-based device for the purpose of programming the OneBox Processor.

Full code documentation can be viewed by opening `/docs/html/index.html` in your web browser
or by running the shortcut at `docs/open-docs.bat`

## Build Instructions

### Windows
1. Install [cmake](https://cmake.org/download/) and Visual Studio extension.
2. Clone the repo:
```bash
git clone https://github.com/DevenMarrero/AmplinkFlashProgrammer.git
```
3. Build:
```bash
cd AmplinkFlashProgrammer
mkdir build && cd build
cmake ..
cmake --build . --config Release
```
4. Copy `ftd2xx.dll` and `libmpsse.dll` from `lib/` to `bin/`.

## Dependencies

The following DLLs are required to run the program
| DLL Name | Purpose | Source |
| -------- | ------- | ------ |
| `ftd2xx.dll` | ftd2xx device driver | Included in `libs/` folder |
| `libmpsse.dll` | SPI/I2C device driver | Included in `libs/` folder |

**Important:** Make sure the DLLs are in the same directory as the executable. They can be copied from `libs/` folder

## Usage

### CLI Syntax

`./AmplinkFlashProgrammer.exe [options]`

### Options

| Option | Description | Default |
| -------| ----------- | ------- |
| `-1 <file>` | Input file for versaClock | clock.hex |
| `-2 <file>` | Input file for Flash 2A | flash_2A.hex |
| `-3 <file>` | Input file for Flash 3A | flash_3A.hex |
| `-4 <file>` | Input file for Flash 4A | flash_4A.hex |
| `-i <addr>` | i2c address of versaClock | 0x6A |
| `-h` | show help message and exit | - |

## Arduino Simulator

`arduino_analyzer.ino` was designed to simulate the flash memory and VersaClock devices. Connecting the SPI and I2C lines of the Arduino UNO to the amplink will allow it to respond to opcodes with the expected addresses and status registers. 
