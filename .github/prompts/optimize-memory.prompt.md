---
mode: agent
description: Analyse and optimise RAM, flash, and stack usage for a Pico SDK project
---

Analyse this Pico SDK project for memory and flash inefficiencies and suggest concrete
improvements.

## Step 1 — Measure current usage

Ask the user to provide the map file and/or `size` output:

```bash
# After building, from the build directory:
arm-none-eabi-size firmware.elf
arm-none-eabi-nm --size-sort --print-size firmware.elf | tail -40
# Or, for a full breakdown:
arm-none-eabi-objdump -h firmware.elf
```

Typical RP2040 budget:
- Flash (XIP): 2 MB — executable code + read-only data
- SRAM: 264 KB — stack, heap, BSS, data

Typical RP2350 budget:
- Flash: 2 MB standard (board-dependent)
- SRAM: 520 KB

## Step 2 — Identify large contributors

Look for:

1. **Large BSS / data sections** — uninitialised or zero-initialised global arrays.
2. **Unused SDK libraries** linked in — check `target_link_libraries` and remove unused ones.
3. **Printf / float formatting** — linking `printf` with float support adds ~20 KB.  Use
   `pico_set_printf_minimal` or `pico_set_binary_type` if float printing is not needed.
4. **Redundant string literals** — deduplicate error strings with a shared string table.
5. **C++ exceptions and RTTI** — disable with `-fno-exceptions -fno-rtti` if not needed.

## Step 3 — Flash optimisation techniques

| Technique | Saving | Risk |
|---|---|---|
| `-Os` instead of `-O2` | 5–20 % flash | Slightly slower code |
| `-flto` (link-time optimisation) | 3–10 % flash | Longer build times |
| `-ffunction-sections -fdata-sections` + `--gc-sections` | 5–15 % | None if tests pass |
| Remove unused features with `#define PICO_STDIO_USB 0` etc. | Varies | Lose feature |
| Use `__attribute__((section(".rodata")))` for lookup tables | 0 (correct placement) | None |

## Step 4 — RAM optimisation techniques

| Technique | Saving | Notes |
|---|---|---|
| Move constants to flash: `const … = { … };` | Per array size | Read-only data should live in flash, not RAM |
| Use `static` for local arrays accessed from a single function | Avoids stack use | May increase BSS; consider carefully |
| Reduce stack sizes to measured high-water mark + safety margin | 0.5–2 KB | Use `pico_set_min_stack_size` |
| Use bit-fields for flags instead of `bool` arrays | Per instance | Slight code size increase |
| Prefer fixed-size pool allocators over `malloc` | Avoids fragmentation | Implementation complexity |

## Step 5 — Linker script hints

```cmake
# Add to your top-level CMakeLists.txt to enable dead-code stripping:
target_compile_options(${PROJECT_NAME} PRIVATE
    -ffunction-sections
    -fdata-sections
)
target_link_options(${PROJECT_NAME} PRIVATE
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)
```

## Step 6 — Prioritised recommendations

After reviewing the user's map file and source, output:

1. **Quick wins** — changes that save the most memory for the least effort.
2. **Architectural changes** — restructuring that would yield significant savings.
3. **Risks** — any trade-offs or regressions to watch out for.

Always verify changes by checking `arm-none-eabi-size` before and after each optimisation.
