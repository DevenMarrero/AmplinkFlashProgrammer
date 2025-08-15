/*!
 * @mainpage Onebox Processor Programmer
 *
 * @section intro_sec Introduction
 * This project provides drivers and programming features to utilize an
 * AmPLink FTDI-based device for the purpose of programming the OneBox Processor.
 * 
 *
 * @section code_sec Files
 * 
 * ### User Programming Files
 * These files contain the core functions that the user can access for the purpose of programming.
 * - @ref programmer.h
 * - @ref fileparser.h
 * - @ref cli.h
 *
 * ### FTDI Driver API's
 * These files provide a clean API for interacting with the ftd2xx and libmpsse libraries. These are internally accessed by `programmer.h`
 * - @ref gpio_driver.h
 * - @ref spi_driver.h
 * - @ref i2c_driver.h
 *
 *
 * @section build_sec Build Instructions
 * 1. Install [cmake](https://cmake.org/download/) and Visual Studio extension.
 * 2. Build
 * - `cd AmplinkFlashProgrammer`
 * - `mkdir build && cd build`
 * - `cmake ..`
 * - `cmake --build && cd build` 
 *
 *
 * @section depend_sec Dependencies
 * DLLS are included in `lib/` folder. Make sure they are copied to the same directory as the 
 * executable before running.
 * - ftd2xx.dll
 * - libmpsse.dll 
 *
 *
 * 
 * 
 * 
 * 
 * 
 * 
*/