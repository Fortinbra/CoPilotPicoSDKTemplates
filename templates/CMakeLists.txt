# Top-level CMakeLists.txt template for a Pico SDK project.
#
# Usage:
#   1. Copy this directory to your project root.
#   2. Replace every occurrence of "my_project" with your project name.
#   3. Set PICO_BOARD to the board you are targeting (pico, pico_w, pico2, …).
#   4. Add sub-directories for each module/driver in src/.
#
# Build:
#   mkdir build && cd build
#   cmake .. -DPICO_SDK_PATH=/path/to/pico-sdk -DPICO_BOARD=pico
#   cmake --build . --parallel

cmake_minimum_required(VERSION 3.17)

# Pull in the Pico SDK CMake helpers.
# PICO_SDK_PATH can be set as an environment variable or a CMake cache variable.
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(my_project
    VERSION 0.1.0
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD   17)
set(CMAKE_CXX_STANDARD 17)

# Initialise the Pico SDK (must be called after project()).
pico_sdk_init()

# ---------------------------------------------------------------------------
# Project-wide compile options
# ---------------------------------------------------------------------------
add_compile_options(
    -Wall
    -Wextra
    -Werror
    -Wno-unused-parameter  # Remove once all parameters are used
    -ffunction-sections
    -fdata-sections
)

add_link_options(
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)

# ---------------------------------------------------------------------------
# Sub-directories
# Each module/driver should have its own CMakeLists.txt.
# ---------------------------------------------------------------------------
add_subdirectory(src)

# ---------------------------------------------------------------------------
# Main firmware target
# ---------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    src/main.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    # Add module libraries here, e.g.:
    # my_driver
    # my_component
)

# Enable stdio via UART (disable USB CDC if not needed, and vice versa).
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME}  0)

# Generate .uf2, .bin, .hex, and .map files alongside the ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# ---------------------------------------------------------------------------
# Host-side unit tests (compiled with the native toolchain)
# Enable with: cmake -DBUILD_HOST_TESTS=ON ..
# ---------------------------------------------------------------------------
option(BUILD_HOST_TESTS "Build host-side unit tests" OFF)
if(BUILD_HOST_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
