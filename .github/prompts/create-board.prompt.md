---
mode: agent
description: Generate a custom board header (board.h) for an RP2040 or RP2350 design on a custom PCB
---

Generate a `board.h` file for a custom PCB that is **not** already included in the Pico SDK's
`src/boards/` directory.  This file tells the SDK about your board's hardware configuration
so that standard SDK defaults (pin assignments, flash size, oscillator frequency, etc.) are
correct without needing to hard-code them in application code.

## Clarifying questions

Answer the following before generating the file.  Skip any that do not apply.

1. **Chip** — RP2040 or RP2350?
2. **Board name** — a short `snake_case` identifier, e.g. `my_sensor_node`.
   This becomes the value of `PICO_BOARD` and the file name (`boards/my_sensor_node.h`).
3. **Crystal / oscillator frequency** — almost always 12 MHz (`12000` kHz); confirm if
   different.
4. **Flash chip and size** — e.g. W25Q128 = 16 MB.  If no external flash (execute-in-place
   from ROM only), say so.
5. **Default UART** (for stdio / debug): instance number and TX / RX GPIO pins.
6. **Default I2C**: instance number and SDA / SCL GPIO pins (if used).
7. **Default SPI**: instance number and SCK / TX / RX / CSn GPIO pins (if used).
8. **Status LED**: GPIO pin number and whether it is active-high or active-low.
   If a WS2812 / NeoPixel is used instead, provide the GPIO pin.
9. **SMPS / power mode**: does the board use the Pico's power-save pin (`PICO_SMPS_MODE_PIN`)?
10. **RP2350 extras** (RP2350 only): binary information flags such as `PICO_RP2350_A2_SUPPORTED`.
11. **Radio Module 2 (RM2) / wireless**: does the board include a CYW43439 (or compatible RM2)
    wireless chipset?  If yes, which GPIO pins are used for the interface?
    The Pico W and Pico 2 W use the following defaults — confirm whether your PCB matches:

    | Signal | Pico W / Pico 2 W default | SDK macro |
    |--------|---------------------------|-----------|
    | WL_REG_ON (power enable) | GPIO 23 | `CYW43_PIN_WL_REG_ON` |
    | WL_DATA (bidirectional SPI data / host-wake) | GPIO 24 | `CYW43_PIN_WL_DATA_OUT` / `CYW43_PIN_WL_HOST_WAKE` |
    | WL_CS (chip select) | GPIO 25 | `CYW43_PIN_WL_CS` |
    | WL_CLK (SPI clock; also ADC3 / VSYS) | GPIO 29 | `CYW43_PIN_WL_CLK` |

    Note: on Pico W / Pico 2 W the status LED is driven through CYW43 GPIO 0
    (`CYW43_DEFAULT_LED_PIN 0`) rather than a direct MCU GPIO, so `PICO_DEFAULT_LED_PIN`
    is not set on those boards.  If your custom board uses the same LED arrangement, say so.

---

## Files to create

### `boards/<board_name>.h`

Place this file in a `boards/` directory at the project root (or wherever your CMake build
points `PICO_BOARD_HEADER_DIRS`).

```c
#pragma once

// ----------------------------------------------------------------------------
// Board definition: <board_name>
// Custom PCB with <chip> — not part of the official Pico SDK board list.
// ----------------------------------------------------------------------------

/** @defgroup board_<board_name> <BoardName> board configuration
 *  @{
 */

// -- Chip selection ----------------------------------------------------------
// Defined by the build system via -DPICO_BOARD=<board_name>.
// Ensure CMakeLists.txt sets:
//   set(PICO_BOARD <board_name> CACHE STRING "")
//   set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/boards CACHE PATH "")

// -- Oscillator --------------------------------------------------------------
#ifndef PICO_XOSC_KHZ
#define PICO_XOSC_KHZ 12000  /**< Crystal frequency in kHz (12000 kHz = 12 MHz). */
#endif

// -- Flash -------------------------------------------------------------------
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (16 * 1024 * 1024)  /**< 16 MB W25Q128. */
#endif

// -- Default UART (stdio / debug) --------------------------------------------
#ifndef PICO_DEFAULT_UART
#define PICO_DEFAULT_UART 0
#endif
#ifndef PICO_DEFAULT_UART_TX_PIN
#define PICO_DEFAULT_UART_TX_PIN 0   /**< GPIO0 = UART0 TX. */
#endif
#ifndef PICO_DEFAULT_UART_RX_PIN
#define PICO_DEFAULT_UART_RX_PIN 1   /**< GPIO1 = UART0 RX. */
#endif

// -- Default I2C -------------------------------------------------------------
#ifndef PICO_DEFAULT_I2C
#define PICO_DEFAULT_I2C 0
#endif
#ifndef PICO_DEFAULT_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SDA_PIN 4   /**< GPIO4 = I2C0 SDA. */
#endif
#ifndef PICO_DEFAULT_I2C_SCL_PIN
#define PICO_DEFAULT_I2C_SCL_PIN 5   /**< GPIO5 = I2C0 SCL. */
#endif

// -- Default SPI -------------------------------------------------------------
#ifndef PICO_DEFAULT_SPI
#define PICO_DEFAULT_SPI 0
#endif
#ifndef PICO_DEFAULT_SPI_SCK_PIN
#define PICO_DEFAULT_SPI_SCK_PIN  18  /**< GPIO18 = SPI0 SCK. */
#endif
#ifndef PICO_DEFAULT_SPI_TX_PIN
#define PICO_DEFAULT_SPI_TX_PIN   19  /**< GPIO19 = SPI0 TX (MOSI). */
#endif
#ifndef PICO_DEFAULT_SPI_RX_PIN
#define PICO_DEFAULT_SPI_RX_PIN   16  /**< GPIO16 = SPI0 RX (MISO). */
#endif
#ifndef PICO_DEFAULT_SPI_CSN_PIN
#define PICO_DEFAULT_SPI_CSN_PIN  17  /**< GPIO17 = SPI0 CSn. */
#endif

// -- LED ---------------------------------------------------------------------
#ifndef PICO_DEFAULT_LED_PIN
#define PICO_DEFAULT_LED_PIN      25  /**< GPIO25 = status LED (active-high). */
#endif
#ifndef PICO_DEFAULT_LED_PIN_INVERTED
#define PICO_DEFAULT_LED_PIN_INVERTED 0  /**< 0 = active-high, 1 = active-low. */
#endif

// (Uncomment if using a WS2812/NeoPixel instead of a plain LED)
// #ifndef PICO_DEFAULT_WS2812_PIN
// #define PICO_DEFAULT_WS2812_PIN 16
// #endif

// -- Radio Module 2 (RM2) / CYW43439 wireless --------------------------------
// Uncomment and adjust this block if the board includes a CYW43439 (or
// compatible RM2) wireless chipset.  The values below match the Pico W and
// Pico 2 W reference design; update GPIO numbers if your PCB differs.
//
// #ifndef CYW43_PIN_WL_REG_ON
// #define CYW43_PIN_WL_REG_ON      23  /**< GPIO23 = CYW43 power enable (WL_REG_ON). */
// #endif
// #ifndef CYW43_PIN_WL_DATA_OUT
// #define CYW43_PIN_WL_DATA_OUT    24  /**< GPIO24 = CYW43 SPI data out (MOSI). */
// #endif
// #ifndef CYW43_PIN_WL_HOST_WAKE
// #define CYW43_PIN_WL_HOST_WAKE   24  /**< GPIO24 = CYW43 host-wake (shared with DATA). */
// #endif
// #ifndef CYW43_PIN_WL_CS
// #define CYW43_PIN_WL_CS          25  /**< GPIO25 = CYW43 SPI chip select. */
// #endif
// #ifndef CYW43_PIN_WL_CLK
// #define CYW43_PIN_WL_CLK         29  /**< GPIO29 = CYW43 SPI clock (also ADC3/VSYS). */
// #endif
//
// When the LED is driven via the CYW43 GPIO rather than a direct MCU pin,
// remove PICO_DEFAULT_LED_PIN above and set the CYW43 LED GPIO instead:
// #ifndef CYW43_DEFAULT_LED_PIN
// #define CYW43_DEFAULT_LED_PIN     0  /**< CYW43 GPIO0 = status LED. */
// #endif

/** @} */
```

> **Tip — RP2350 only**: add the following if the silicon revision supports it:
> ```c
> #define PICO_RP2350_A2_SUPPORTED 1
> ```

---

### CMakeLists.txt integration

Add these lines **before** `pico_sdk_init()` in your top-level `CMakeLists.txt`:

```cmake
# Tell the SDK where to find custom board headers.
set(PICO_BOARD <board_name> CACHE STRING "Target board")
set(PICO_BOARD_HEADER_DIRS "${CMAKE_SOURCE_DIR}/boards" CACHE PATH
    "Directory containing custom board headers")
```

After `pico_sdk_init()`, verify that the correct board was picked up:

```cmake
message(STATUS "Building for board: ${PICO_BOARD}")
```

---

## Rules

- Keep every `#define` wrapped in `#ifndef` / `#endif` so that values can still be overridden
  from the CMake command line (`-DPICO_DEFAULT_LED_PIN=12`).
- Use only GPIO numbers — never schematic net names — in this file.
- Do not add application-level pin definitions here; those belong in a project-specific
  `pin_map.h`.  `board.h` is strictly SDK defaults.
- All flash-size constants must be exact powers of two expressed as byte counts.
- If the board has no external flash (bare chip with ROM-only boot), set
  `PICO_FLASH_SIZE_BYTES 0` and document why.
- `#pragma once` is required; do **not** use `#ifndef` header guards.
- For boards with a CYW43439 / RM2: the four RM2 GPIO pins (`WL_REG_ON`, `WL_DATA`,
  `WL_CS`, `WL_CLK`) are reserved and must not be assigned to any other peripheral.
  If the LED is driven through the CYW43, omit `PICO_DEFAULT_LED_PIN` and define
  `CYW43_DEFAULT_LED_PIN` instead.  Link with `pico_cyw43_arch_*` rather than any
  direct GPIO calls to that LED.
