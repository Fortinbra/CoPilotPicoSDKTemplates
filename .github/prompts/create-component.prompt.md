---
mode: agent
description: Scaffold a new hardware abstraction component for a Pico SDK project
---

Create a new hardware abstraction component for this Pico SDK project.

## What to provide

Before generating code, confirm the following with the user if not already specified:

1. **Component name** — e.g. `led_matrix`, `temperature_sensor`, `rotary_encoder`
2. **Target hardware** — RP2040 or RP2350, and any relevant Pico board variant
3. **Interface type** — GPIO, SPI, I2C, UART, PIO, or internal (no hardware)
4. **Required operations** — initialise, read, write, enable, disable, etc.

## Files to create

Generate the following files, substituting `<name>` with the snake_case component name:

### `src/<name>/<name>.h`

```c
#pragma once

/**
 * @file <name>.h
 * @brief Public interface for the <Name> component.
 *
 * <One-sentence description of the component's responsibility.>
 */

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"

/** Configuration passed to <name>_init(). */
typedef struct {
    /* TODO: add hardware-specific configuration fields */
} <Name>Config;

/** Opaque handle returned by <name>_init(). */
typedef struct <Name>Handle <Name>Handle;

/**
 * @brief Initialise the <name> component.
 * @param[in]  config  Pointer to a populated <Name>Config struct.
 * @param[out] handle  On success, receives a pointer to the component handle.
 * @return PICO_OK on success, or a PICO_ERROR_* code on failure.
 */
int <name>_init(const <Name>Config *config, <Name>Handle **handle);

/**
 * @brief Release all resources held by the component.
 * @param[in] handle  Handle obtained from <name>_init().
 */
void <name>_deinit(<Name>Handle *handle);
```

### `src/<name>/<name>.c`

```c
#include "<name>.h"
#include <stdlib.h>
#include "pico/stdlib.h"

struct <Name>Handle {
    <Name>Config config;
    /* TODO: add internal state fields */
};

int <name>_init(const <Name>Config *config, <Name>Handle **handle) {
    if (config == NULL || handle == NULL) {
        return PICO_ERROR_INVALID_ARG;
    }

    <Name>Handle *h = calloc(1, sizeof(<Name>Handle));
    if (h == NULL) {
        return PICO_ERROR_NO_DATA; /* no memory */
    }

    h->config = *config;

    /* TODO: initialise hardware */

    *handle = h;
    return PICO_OK;
}

void <name>_deinit(<Name>Handle *handle) {
    if (handle == NULL) {
        return;
    }
    /* TODO: release hardware resources */
    free(handle);
}
```

### `src/<name>/CMakeLists.txt`

```cmake
add_library(<name> STATIC
    <name>.c
)

target_include_directories(<name> PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(<name> PUBLIC
    pico_stdlib
)

target_compile_options(<name> PRIVATE
    -Wall -Wextra -Werror
)
```

## Rules

- Follow the coding style defined in `.github/copilot-instructions.md`.
- Every public symbol must be prefixed with the component name.
- Use `PICO_OK` / `PICO_ERROR_*` for error returns.
- Claim SDK resources (DMA, PIO state machines) with `*_claim` APIs.
- Do not use dynamic allocation unless unavoidable; if used, document why.
