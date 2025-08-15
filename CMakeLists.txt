cmake_minimum_required(VERSION 3.10)
project(AmplinkFlashProgrammer C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/headers)
link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB SOURCES "src/*.c")

add_executable(AmplinkFlashProgrammer ${SOURCES})

# Link Libraries
target_link_libraries(AmplinkFlashProgrammer ftd2xx libmpsse)