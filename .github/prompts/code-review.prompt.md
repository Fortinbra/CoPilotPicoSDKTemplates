---
mode: agent
description: Review firmware code for a Pico SDK C/C++ project
---

Perform a thorough code review of the selected files or the changes in this pull request.

Focus on the following areas in order of priority:

## 1. Correctness

- Are there any off-by-one errors, integer overflows, or sign-extension bugs?
- Are pointer dereferences guarded against `NULL`?
- Are all return values of SDK functions checked?
- Is shared state between cores or between main code and ISRs properly protected with
  `critical_section_t`, `mutex_t`, or `spin_lock_t`?
- Are DMA transfers properly fenced with `dma_channel_wait_for_finish_blocking` or an equivalent?

## 2. Resource management

- Are SDK hardware resources (DMA channels, PIO state machines, IRQ numbers) claimed with
  `*_claim` / `hw_claim_or_assert` APIs?
- Are claimed resources released (`*_unclaim`) on error paths and on de-initialisation?
- Is there any unintended dynamic allocation (`malloc`, `new`) in ISR or time-critical contexts?

## 3. SOLID and architecture

- Does each module have a single, well-defined responsibility?
- Are hardware dependencies injected through configuration structs or interface pointers rather
  than reached through globals?
- Are interfaces narrow enough that callers are not forced to depend on methods they do not use?

## 4. Pico SDK conventions

- Are clock frequencies obtained from `clock_get_hz()` rather than hard-coded magic numbers?
- Is `stdio_init_all()` called before any `printf` usage?
- For multicore code: is core 1 launched via `multicore_launch_core1()` and are inter-core
  communications going through `queue_t`?
- Is time-critical code annotated with `__not_in_flash_func` / `__time_critical_func` where
  appropriate?

## 5. Portability between RP2040 and RP2350

- Does the code use `#if defined(PICO_RP2350)` conditionals only where truly necessary?
- Are memory sizes, peripheral counts, and IRQ numbers taken from SDK macros rather than
  hard-coded?

## 6. Code style

- Does the code follow the conventions in `.github/copilot-instructions.md` (naming, line
  length, brace placement, Doxygen comments)?
- Are all public symbols prefixed with the module name?
- Does every header have `#pragma once`?

## Output format

For each issue found, provide:

```
File: <path>
Line: <line number(s)>
Severity: [Critical | Major | Minor | Suggestion]
Category: <one of the categories above>
Issue: <concise description>
Suggestion: <how to fix it>
```

End with a brief summary of overall code quality and the most important items to address.
