# CoPilotPicoSDKTemplates

A collection of reusable GitHub Copilot instructions, prompt templates, agents, and git hooks
for firmware projects built on the
[Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) (RP2040 / RP2350).

Designed for use with the
[Raspberry Pi Pico VS Code extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico),
which is supported on **Windows, macOS, and Linux** and handles toolchain setup, CMake
configuration, and flashing automatically.

---

## Purpose

This repository is an intentionally **project-agnostic** starting point.  Use it as a
[GitHub template repository](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template)
to seed new Pico SDK projects with Copilot guidance from day one, or copy individual files into
an existing project.

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
    create-board.prompt.md       # Generate a board.h for a custom PCB
    code-review.prompt.md        # Embedded firmware code review
    add-unit-test.prompt.md      # Add unit tests for firmware code
    debug-hardfault.prompt.md    # Debug a HardFault / crash
    optimize-memory.prompt.md    # Memory & flash optimisation guidance
hooks/
  pre-commit                # Git pre-commit hook (style & sanity checks)
  commit-msg                # Git commit-msg hook (conventional commits)
  install-hooks.sh          # Helper script to install the hooks
```

---

## Quick start

### New project — use as a template

Click **Use this template → Create a new repository** on GitHub.  The new repo will contain all
the Copilot guidance and hooks out of the box.

### Existing project — copy what you need

#### Copilot instructions

```bash
cp .github/copilot-instructions.md  <your-project>/.github/copilot-instructions.md
```

Edit the file to add project-specific context (board name, peripherals, memory budget, etc.).

#### Prompt templates

```bash
cp -r .github/prompts  <your-project>/.github/prompts
```

Prompt files appear in the **GitHub Copilot Chat** slash-command list when you open the repo in
VS Code. Invoke them with `/create-component`, `/create-board`, `/code-review`, etc.

#### Copilot agents

```bash
cp -r .github/agents  <your-project>/.github/agents
```

#### Git hooks

```bash
cp -r hooks  <your-project>/hooks
bash <your-project>/hooks/install-hooks.sh
```

---

## Contributing

Improvements that make the templates more useful across a broad range of Pico SDK projects are
welcome.  Please open an issue or pull request.

---

## License

[MIT](LICENSE)
