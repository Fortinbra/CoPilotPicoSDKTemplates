---
mode: agent
description: Add host-side unit tests for a Pico SDK firmware module
---

Add host-compilable unit tests for the specified firmware module.

## Approach

Pico SDK firmware can be tested on the host (x86/x64 Linux or macOS) by:

1. Isolating the logic under test behind an abstraction boundary.
2. Providing stub/mock implementations of SDK hardware calls.
3. Compiling the test binary with the host toolchain (not `arm-none-eabi-gcc`).

## What to provide

1. **Module to test** — the `.c` / `.h` file(s) containing the logic.
2. **Test framework in use** — Unity (default), CMock, or Catch2.
3. **What behaviours to test** — happy path, error paths, boundary conditions.

## Files to create

Substitute `<name>` with the module name.

### `tests/<name>/test_<name>.c` (Unity example)

```c
#include "unity.h"
#include "<name>.h"
/* Include stubs for any hardware-dependent calls */

void setUp(void) {
    /* Called before each test. Initialise the module under test. */
}

void tearDown(void) {
    /* Called after each test. Clean up. */
}

void test_<name>_init_returns_ok_with_valid_config(void) {
    <Name>Config cfg = { /* valid configuration */ };
    <Name>Handle *handle = NULL;
    TEST_ASSERT_EQUAL_INT(PICO_OK, <name>_init(&cfg, &handle));
    TEST_ASSERT_NOT_NULL(handle);
    <name>_deinit(handle);
}

void test_<name>_init_returns_error_with_null_config(void) {
    <Name>Handle *handle = NULL;
    TEST_ASSERT_NOT_EQUAL(PICO_OK, <name>_init(NULL, &handle));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_<name>_init_returns_ok_with_valid_config);
    RUN_TEST(test_<name>_init_returns_error_with_null_config);
    return UNITY_END();
}
```

### `tests/<name>/CMakeLists.txt`

```cmake
# Host-side unit test — compiled with the native toolchain.
cmake_minimum_required(VERSION 3.17)
project(test_<name> C)

# Locate Unity (adjust path to match your project layout).
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/Unity Unity)

add_executable(test_<name>
    test_<name>.c
    ${CMAKE_SOURCE_DIR}/src/<name>/<name>.c
)

target_include_directories(test_<name> PRIVATE
    ${CMAKE_SOURCE_DIR}/src/<name>
    ${CMAKE_SOURCE_DIR}/tests/stubs
)

target_link_libraries(test_<name> PRIVATE unity)

add_test(NAME test_<name> COMMAND test_<name>)
```

### `tests/stubs/pico/stdlib.h` (minimal stub, if not already present)

```c
#pragma once
/* Minimal stub so firmware headers compile on the host. */
#include <stdint.h>
#include <stdbool.h>

#define PICO_OK               0
#define PICO_ERROR_INVALID_ARG (-1)
#define PICO_ERROR_NO_DATA    (-2)
#define PICO_ERROR_TIMEOUT    (-3)

typedef unsigned int uint;
```

## Test-writing guidelines

- Test **one behaviour per test function**.
- Name tests: `test_<module>_<scenario>_<expected_outcome>`.
- Test all **error paths**, not only the happy path.
- Mock or stub every SDK hardware call so tests run without hardware.
- Aim for tests that execute in < 1 ms each — no `sleep_ms` in host tests.
- After writing tests, run them and make sure they pass before committing.
