# CoPilotPicoSDKTemplates

A collection of reusable GitHub Copilot instructions, prompt templates, agents, git hooks, and
project templates for firmware projects built on the
[Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) (RP2040 / RP2350).

---

## Purpose

This repository is an intentionally **project-agnostic** starting point.  Copy the pieces you
need into your own Pico SDK project and customise them to match your specific hardware and
requirements.

The goals are to:

- Keep code **clean and maintainable** across the lifetime of a firmware project.
- Apply **SOLID principles** wherever they make sense in an embedded C/C++ context.
- Follow **Pico SDK conventions** so that code integrates naturally with the SDK and its build
  system.
- Target both the **RP2040** and **RP2350** families without favouring one over the other.

---

## Repository layout

```
.github/
  copilot-instructions.md   # Main GitHub Copilot custom instructions
  agents/
    firmware-reviewer.md    # Copilot agent – firmware code reviewer
    sdk-expert.md           # Copilot agent – Pico SDK expert
  prompts/
    create-component.prompt.md   # Scaffold a new HAL/driver component
    create-driver.prompt.md      # Scaffold a peripheral driver
    code-review.prompt.md        # Embedded firmware code review
    add-unit-test.prompt.md      # Add unit tests for firmware code
    debug-hardfault.prompt.md    # Debug a HardFault / crash
    optimize-memory.prompt.md    # Memory & flash optimisation guidance
  workflows/
    ci.yml                  # GitHub Actions CI skeleton
hooks/
  pre-commit                # Git pre-commit hook (style & sanity checks)
  commit-msg                # Git commit-msg hook (conventional commits)
  install-hooks.sh          # Helper script to install the hooks
templates/
  CMakeLists.txt            # Top-level CMake template
  src/
    main.c                  # Application entry-point template
    module/
      module.h              # Header template (include guard, Doxygen stub)
      module.c              # Source template
```

---

## Quick start

### 1. Copy the Copilot instructions

```bash
cp .github/copilot-instructions.md  <your-project>/.github/copilot-instructions.md
```

Edit the file to add project-specific context (board name, peripherals, memory budget, etc.).

### 2. Copy the prompt templates

```bash
cp -r .github/prompts  <your-project>/.github/prompts
```

Prompt files appear in the **GitHub Copilot Chat** slash-command list when you open the repo in
VS Code. Invoke them with `/create-component`, `/code-review`, etc.

### 3. Install the git hooks

```bash
cp hooks/ <your-project>/hooks/
bash <your-project>/hooks/install-hooks.sh
```

### 4. Use the project templates

Copy `templates/` into your project root and rename files as appropriate.  The CMakeLists.txt
template assumes the standard Pico SDK `pico_sdk_import.cmake` mechanism.

---

## Contributing

Improvements that make the templates more useful across a broad range of Pico SDK projects are
welcome.  Please open an issue or pull request.

---

## License

[MIT](LICENSE)
