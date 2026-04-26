---
mode: agent
description: Scaffold a new peripheral driver for a Pico SDK project
---

Create a new low-level peripheral driver for this Pico SDK project.

## Difference from a component

A **driver** wraps a single SDK peripheral (UART, SPI, I2C, PWM, ADC, PIO, …) and exposes a
thin, hardware-specific API.  A **component** (see `create-component.prompt.md`) sits above the
driver and provides a higher-level, hardware-agnostic interface.

## What to provide

Confirm the following before generating code:

1. **Peripheral** — e.g. `spi0`, `i2c1`, `uart0`, `adc`, `pwm_slice_0`
2. **Purpose** — e.g. "SPI driver for a W25Q128 flash chip"
3. **Transfer mode** — polling, interrupt-driven, or DMA
4. **Target chip** — RP2040 or RP2350

## Files to create

Substitute `<periph>` with the snake_case driver name (e.g. `spi_flash`, `i2c_sensor`).

### `src/drivers/<periph>/<periph>_driver.h`

```c
#pragma once

/**
 * @file <periph>_driver.h
 * @brief Low-level driver for <peripheral description>.
 */

#include <stdint.h>
#include <stddef.h>
#include "pico/stdlib.h"
#include "hardware/<sdk_header>.h"

/** Pin and peripheral configuration for the <Periph> driver. */
typedef struct {
    uint    instance;   /**< SDK peripheral instance number */
    uint    pin_<x>;    /**< GPIO pin number for signal <X> */
    uint32_t baud_rate; /**< Baud / clock rate in Hz */
} <Periph>DriverConfig;

/**
 * @brief Initialise the peripheral and configure GPIO pins.
 * @param[in] config  Pointer to driver configuration.
 * @return PICO_OK on success.
 */
int <periph>_driver_init(const <Periph>DriverConfig *config);

/**
 * @brief De-initialise and release the peripheral.
 */
void <periph>_driver_deinit(void);

/**
 * @brief Write bytes to the peripheral.
 * @param[in] data    Buffer to transmit.
 * @param[in] length  Number of bytes to transmit.
 * @return PICO_OK on success, PICO_ERROR_TIMEOUT on timeout.
 */
int <periph>_driver_write(const uint8_t *data, size_t length);

/**
 * @brief Read bytes from the peripheral.
 * @param[out] buffer  Receive buffer.
 * @param[in]  length  Number of bytes to read.
 * @return PICO_OK on success, PICO_ERROR_TIMEOUT on timeout.
 */
int <periph>_driver_read(uint8_t *buffer, size_t length);
```

### `src/drivers/<periph>/<periph>_driver.c`

Implement the functions above using the appropriate Pico SDK hardware API.

```c
#include "<periph>_driver.h"
#include "hardware/<sdk_header>.h"

/* Static handle — one instance per driver. */
static struct {
    <Periph>DriverConfig config;
    bool initialised;
} s_driver;

int <periph>_driver_init(const <Periph>DriverConfig *config) {
    if (config == NULL) {
        return PICO_ERROR_INVALID_ARG;
    }
    /* TODO: initialise SDK peripheral */
    s_driver.config      = *config;
    s_driver.initialised = true;
    return PICO_OK;
}

void <periph>_driver_deinit(void) {
    if (!s_driver.initialised) {
        return;
    }
    /* TODO: de-initialise SDK peripheral */
    s_driver.initialised = false;
}

int <periph>_driver_write(const uint8_t *data, size_t length) {
    if (!s_driver.initialised || data == NULL) {
        return PICO_ERROR_INVALID_ARG;
    }
    /* TODO: implement transfer */
    return PICO_OK;
}

int <periph>_driver_read(uint8_t *buffer, size_t length) {
    if (!s_driver.initialised || buffer == NULL) {
        return PICO_ERROR_INVALID_ARG;
    }
    /* TODO: implement transfer */
    return PICO_OK;
}
```

### `src/drivers/<periph>/CMakeLists.txt`

```cmake
add_library(<periph>_driver STATIC
    <periph>_driver.c
)

target_include_directories(<periph>_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(<periph>_driver PUBLIC
    pico_stdlib
    hardware_<sdk_library>
)

target_compile_options(<periph>_driver PRIVATE
    -Wall -Wextra -Werror
)
```

## Rules

- One driver per SDK peripheral instance.
- Claim the peripheral with `<peripheral>_claim()` / `hw_claim_or_assert()` where applicable.
- Never block for longer than a configurable timeout; return `PICO_ERROR_TIMEOUT`.
- Prefer DMA for transfers larger than 4 bytes where the peripheral supports it.
- ISR handlers must be placed in SRAM: `__isr __not_in_flash_func(<name>_irq_handler)`.
