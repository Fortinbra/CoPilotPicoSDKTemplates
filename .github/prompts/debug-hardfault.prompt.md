---
mode: agent
description: Help diagnose a HardFault, crash, or unexpected reset on an RP2040/RP2350 target
---

Help me debug a HardFault, crash, watchdog reset, or other unexpected fault condition on this
Pico SDK project.

## Step 1 — Gather information

Ask the user for the following if not already provided:

1. **Target chip**: RP2040 or RP2350?
2. **Symptoms**: HardFault LED flash pattern? Watchdog reset? Silent lockup?
3. **When does it occur?** On boot, after a specific operation, under heavy load?
4. **Debug output available?** UART printf, USB CDC, or no output?
5. **Relevant code sections** — paste the crash site or the function believed to be involved.
6. **Stack trace or register dump** — if a debugger (OpenOCD + GDB) is attached, provide the
   output of `info registers`, `bt`, and `x/16xw $sp`.

## Step 2 — Common RP2040/RP2350 fault causes

Walk through each of these with the user:

### Stack overflow
- Default stack size is 2 KB per core.  Large local arrays, deep recursion, or ISR + task
  stacks that overlap will corrupt the stack.
- Check: `ulTaskGetStackHighWaterMark()` (FreeRTOS) or inspect stack pointer vs. stack base.
- Fix: increase `PICO_STACK_SIZE` in CMakeLists or move large buffers to static/global.

### NULL / invalid pointer dereference
- Dereferencing `NULL` or an uninitialised pointer causes a BusFault or HardFault.
- Check: all pointer arguments for `NULL` before use; initialise all pointers to `NULL`.

### Accessing flash during a flash write/erase
- The RP2040 XIP cache is disabled during flash programming.  Any code or data fetch from
  flash during this window will fault.
- Fix: ensure the flash write sequence and all functions it calls are in SRAM
  (`__not_in_flash_func`), and all interrupts that could trigger flash accesses are disabled
  during the operation.

### ISR stack corruption
- An ISR that uses more stack than available will overflow into adjacent memory.
- Fix: keep ISRs minimal; move work to a deferred task or IRQ handler flag.

### Unaligned memory access
- Cortex-M0+ (RP2040) does **not** support unaligned word accesses — this will HardFault.
- Cortex-M33 (RP2350) supports unaligned accesses but they are slower; prefer aligned access.
- Fix: use `memcpy` for unaligned reads/writes; check struct packing (`__attribute__((packed))`).

### DMA writing to read-only memory / out-of-bounds
- A DMA channel configured with a bad destination address will silently corrupt memory.
- Fix: validate DMA destination before starting the transfer.

### Multicore race condition
- Core 0 and Core 1 accessing the same peripheral or memory without synchronisation.
- Fix: protect shared state with `critical_section_t`, `mutex_t`, or `spin_lock_t`.

## Step 3 — GDB diagnostic commands

If a debugger is available, run:

```gdb
# After halting at the fault:
info registers          # Show all CPU registers
print/x $pc             # Faulting PC
print/x $lr             # Link register (return address)
print/x $sp             # Stack pointer
x/32xw $sp              # Dump stack contents
bt                      # Backtrace (if symbols available)

# RP2040 Cortex-M0+: read fault status from SCB
x/1xw 0xE000ED28        # CFSR (Configurable Fault Status Register)
x/1xw 0xE000ED2C        # HFSR (HardFault Status Register)
x/1xw 0xE000ED34        # MMFAR (MemManage Fault Address Register)
x/1xw 0xE000ED38        # BFAR (BusFault Address Register)
```

## Step 4 — Add a fault handler for diagnostics

If no debugger is available, suggest adding a custom HardFault handler that captures registers
and outputs them via UART before resetting:

```c
// Place in SRAM so it executes even if flash is inaccessible.
__attribute__((naked)) __not_in_flash_func(isr_hardfault)(void) {
    __asm volatile (
        "tst lr, #4          \n"
        "ite eq              \n"
        "mrseq r0, msp       \n"
        "mrsne r0, psp       \n"
        "ldr r1, =hardfault_handler_c \n"
        "bx r1               \n"
    );
}

void __not_in_flash_func(hardfault_handler_c)(uint32_t *stack_frame) {
    printf("HardFault!\n");
    printf("  PC  = 0x%08lx\n", stack_frame[6]);
    printf("  LR  = 0x%08lx\n", stack_frame[5]);
    printf("  SP  = 0x%08lx\n", (uint32_t)stack_frame);
    printf("  R0  = 0x%08lx\n", stack_frame[0]);
    printf("  CFSR= 0x%08lx\n", *(volatile uint32_t *)0xE000ED28);
    while (true) { tight_loop_contents(); }
}
```

## Step 5 — Summarise findings

After gathering the information above, provide:

1. The most likely root cause.
2. The minimal code change required to fix it.
3. Defensive measures to prevent similar faults in future.
