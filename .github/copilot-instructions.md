# GitHub Copilot Instructions — Pico SDK Firmware Project

These instructions apply to all C and C++ code in this repository.  They guide GitHub Copilot
toward producing clean, maintainable firmware that is idiomatic for the
[Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) and suitable for both the
**RP2040** and **RP2350** families of microcontrollers.

---

## Development environment

The primary development environment is the
**[Raspberry Pi Pico VS Code extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico)**,
which is supported on **Windows, macOS, and Linux** and handles toolchain installation,
CMake configuration, SDK management, and flashing automatically.

- When suggesting build steps or toolchain setup, **always give the VS Code extension workflow
  first**, followed by the equivalent command-line instructions for CI or non-VS-Code users.
- On Windows the extension installs the `arm-none-eabi-gcc` toolchain and CMake into an
  isolated location; do **not** assume these are on `PATH` — always use the VS Code tasks or
  the `cmake --preset` approach the extension generates.
- For flashing, prefer the extension's one-click "Run" button or drag-and-drop UF2 over
  platform-specific `openocd` command lines, unless the user explicitly asks for the latter.

---

## Language and standard

- Source files use **C17** (`.c` / `.h`) or **C++17** (`.cpp` / `.hpp`).
- The build system is **CMake** with the Pico SDK's `pico_sdk_import.cmake` helper.
- Toolchain is **arm-none-eabi-gcc** (GCC 12+).

---

## Coding style

- Indent with **4 spaces** — no tabs.
- Opening brace on the **same line** as the control statement or function signature.
- Maximum line length: **100 characters**.
- Use `snake_case` for variables, functions, and file names.
- Use `UPPER_SNAKE_CASE` for macros and compile-time constants.
- Use `PascalCase` for type names (`typedef struct`, `typedef enum`, class names).
- Prefix every public symbol with the module name, e.g. `uart_init()`, `UartConfig`.
- Every header file must have a **`#pragma once`** guard (preferred over `#ifndef` guards).
- All `#include` paths must be **relative to the project root** or use angle brackets for system
  and SDK headers.

---

## SOLID principles in embedded C/C++

### Single Responsibility
Each source file / module is responsible for **one hardware peripheral or one logical
abstraction**.  A `uart.c` file configures and drives the UART; it does not parse application
protocols.

### Open/Closed
Extend behaviour through **function-pointer tables**, **vtable structs**, or C++ virtual
interfaces rather than modifying existing driver code.  Example:

```c
typedef struct {
    void (*send)(const uint8_t *data, size_t len);
    int  (*recv)(uint8_t *buf, size_t max_len);
} CommInterface;
```

### Liskov Substitution
Any concrete driver that satisfies a `CommInterface` must honour the **full contract** of that
interface — including error-return semantics and buffer ownership rules.

### Interface Segregation
Define **narrow interfaces**.  A read-only sensor driver should not be forced to implement a
`write` method.  Split large interfaces into smaller, focused ones.

### Dependency Inversion
High-level application code depends on **abstract interfaces**, not on concrete SDK calls.
Inject hardware dependencies (e.g. the SPI instance to use) at initialisation time, not by
reaching into global state.

---

## RP2040 / RP2350 hardware considerations

- **Dual-core (RP2040) / dual-core Cortex-M33 (RP2350)**: clearly document which core owns each
  peripheral.  Use the SDK's `multicore_launch_core1()` pattern only once and keep core
  interactions through `queue_t` or `mutex_t`.
- **Memory layout**: SRAM is limited (264 KB on RP2040, 520 KB on RP2350).  Prefer stack
  allocation for temporary buffers; use `static` for long-lived data only when necessary.  Mark
  time-critical code and read-only data with the correct SDK placement macros (`__not_in_flash_func`,
  `__in_flash`, etc.).
- **DMA**: prefer DMA for bulk data transfers; never busy-wait in an ISR.
- **PIO**: encapsulate PIO programs in their own module.  Keep the `.pio` assembly file
  alongside its C wrapper.
- **Clocks**: never use magic numbers for clock frequencies.  Use SDK constants
  (`clock_get_hz(clk_sys)`) or named `#define`s.
- **Flash XIP**: be aware that code executing from flash is subject to cache stalls.  Interrupt
  handlers that must respond quickly should live in SRAM (`__isr __not_in_flash_func`).

---

## Error handling

- Functions that can fail **must** return an error code or a status type — never silently
  discard errors.
- Use `pico_error_codes` / `PICO_OK` / `PICO_ERROR_*` where appropriate.
- `assert()` is acceptable for programming errors (precondition violations) in debug builds;
  use a dedicated `PANIC()` or `tight_loop_contents()` for unrecoverable runtime errors in
  release builds.
- Never call `printf` in production/release firmware unless UART stdio is explicitly configured.

---

## Resource management

- Claim SDK hardware resources (DMA channels, PIO state machines, IRQs) with the SDK's
  `*_claim` / `*_unclaim` APIs so that resource conflicts are caught early.
- Initialise all variables before use — rely on zero-initialisation of static/global variables
  only where explicitly required.
- Prefer **stack-allocated** structs passed by pointer over heap allocation (`malloc`/`new`) in
  firmware.  If dynamic allocation is necessary, use a fixed-size pool allocator.

---

## Testing

- Unit-testable logic (parsing, state machines, data processing) should be written in
  hardware-independent C/C++ files that can be compiled and tested on the host.
- Hardware-dependent code should be isolated behind thin abstraction layers so that mocks can be
  substituted during host-side tests.
- Use **Unity** or **CMock** for host-side unit tests (or the test framework already present in
  the project).
- Integration tests that must run on hardware should use the Pico SDK's `pico_test` helper or a
  custom serial-based test runner.

---

## CMake conventions

- Each module lives in its own subdirectory with its own `CMakeLists.txt`.
- Library targets use `pico_add_library` (SDK-style) or `add_library`.
- Link dependencies with `target_link_libraries` — never with `link_libraries` or global flags.
- Enable `pico_enable_stdio_uart` / `pico_enable_stdio_usb` explicitly; disable the one not in
  use.
- Add compiler warnings: at minimum `-Wall -Wextra -Werror` for all project targets.

---

## Documentation

- Every public function and type must have a **Doxygen** comment block (brief + param + return).
- Module-level overview comments go at the top of the header file.
- Keep comments factual and brief; prefer self-documenting names over comments that simply
  restate the code.

---

## Clarifying questions and alternatives

When a request would produce code or configuration that conflicts with any guideline in this
file, **do not silently comply and do not silently refuse**.  Instead:

1. **Identify the conflict** — briefly explain which guideline is affected and why the requested
   approach is problematic (e.g. introduces dynamic allocation, bypasses a resource-claim API,
   ignores error returns).
2. **Ask a clarifying question** — check whether there is a specific constraint or use-case that
   justifies the exception, so that the advice can be tailored appropriately.
3. **Suggest a conforming alternative** — offer at least one alternative that satisfies the
   original intent while staying within the guidelines.

Example response pattern:

> The approach you described uses `malloc` inside an ISR, which can cause priority inversion and
> non-deterministic latency (see *Resource management* in the project guidelines).
>
> Could you clarify whether this ISR must allocate variable-length data at runtime, or whether
> the maximum size is known at compile time?  If the size is bounded, a statically allocated
> ring-buffer avoids the allocation entirely — happy to scaffold that instead.

---

## What Copilot should NOT do

- Do **not** suggest `HAL_Delay` or STM32 HAL calls — this is a Pico SDK project.
- Do **not** use `std::cout`, `std::cin`, or other host-only C++ I/O in firmware code.
- Do **not** introduce dynamic memory allocation without a clear justification.
- Do **not** leave placeholder `TODO` comments without a corresponding GitHub issue reference.
- Do **not** disable compiler warnings to silence errors.
