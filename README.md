<h1>
<picture>
  <source media="(prefers-color-scheme: dark)" srcset=".github/brand/header-dark.svg">
  <img src=".github/brand/header-light.svg" alt="Xila Native C app template — A C starter for your first native Xila application." width="1280">
</picture>
</h1>

[Documentation](https://www.xila.dev/docs/) ·
[Community](https://matrix.to/#/#xila:anneraud.fr)

Build a small C application for the Xila embedded OS. The starter prints a
greeting and its command-line arguments to stdout. Pass `--fail` as the first
argument to print a diagnostic to stderr and return status 1. Otherwise the app
returns status 0.

## Requirements

Install [mise](https://mise.jdx.dev/) and the host tools. On Debian or Ubuntu,
run the following commands.

```sh
sudo apt-get update
sudo apt-get install git clang lld llvm cmake make zstd python3
# Install the compiler runtime for each embedded target you intend to build.
sudo apt-get install gcc-arm-none-eabi
sudo apt-get install gcc-riscv64-unknown-elf
```

Use CMake 3.20 or newer, GNU tar, and Python 3.12 or newer. If the distribution
ships an older Python, set `UV_PYTHON=3.12` in your environment so uv can install
and select Python 3.12. The RISC-V toolchain needs the `rv32imac` / `ilp32`
multilib. CI delegates package installation to the pinned Core checkout's
`ci:packages-sdk*` tasks.

The first SDK build needs network access for the pinned Core source, the mise
and uv tools, and Core's pinned picolibc source. Keep the caches and the checkout
under `target/core/` to rebuild offline after the first build.

## Create your application

1. Select **Use this template** on GitHub and clone your new repository, or
   clone this repository directly.
2. Change `app_name` in `mise.toml` to the name of your executable.
3. Edit `src/main.c` to implement your application.

## Build and check

Review the configuration before trusting it. The SDK task fetches Core at the
exact `core_revision` in `mise.toml` and runs Core's SDK build.

```sh
mise trust
mise run sdk:build
mise run build
mise run check
```

The default target is Linux x86-64. Find the unpacked SDK through
`target/sdk/<target>/current/` and the application at
`target/sdk/<target>/current/build/<app_name>.elf`. CMake also writes
`compile_commands.json` alongside the executable. Rerun `build` and `check`
after editing the C source.

Set `XILA_SDK_TARGET` for all three tasks to build for another target.

```sh
export XILA_SDK_TARGET=thumbv7em-none-eabihf
mise run sdk:build
mise run build
mise run check
```

| Target | Xila platform |
| --- | --- |
| `x86_64-unknown-linux-gnu` | Linux x86-64 host |
| `thumbv7em-none-eabihf` | Arm Cortex-M4F, hard float |
| `riscv32imac-unknown-none-elf` | RV32IMAC, soft float |

Run `mise run format:c` to format the tracked C sources and headers.

## Run through Core's host loader

No released Xila build runs native executables yet. Platform support is tracked
in [xila-project/platforms#115](https://github.com/xila-project/platforms/issues/115).

Today you can exercise the Linux x86-64 ELF through Core's
[SDK host tests](https://github.com/xila-project/core/blob/main/libraries/executable/tests/sdk.rs).
In a separate Core checkout at the SDK revision, run
`cargo test --locked -p xila-executable --features native-dev --test sdk`.
To load your own ELF, use the same test helpers to register `NativeFormat` and
install the executable in the test VFS. Pass its VFS path and arguments to
`execute_with`, redirect stdout and stderr to test files, and use `join` to
read the exit status.

## Update the SDK pin

Look up Core's main revision with
`git ls-remote https://github.com/xila-project/core.git refs/heads/main`.
Review the revision and put its full SHA in `core_revision` in `mise.toml`.
Verify the pin on all three targets before committing it.

```sh
for target in x86_64-unknown-linux-gnu thumbv7em-none-eabihf riscv32imac-unknown-none-elf; do
  XILA_SDK_TARGET="$target" mise run sdk:build &&
    XILA_SDK_TARGET="$target" mise run build &&
    XILA_SDK_TARGET="$target" mise run check || exit 1
done
git add mise.toml
git commit -m "Update the Xila Core SDK pin"
```

`sdk:build` owns SDK acquisition and stages the toolchain and checker at the
same paths on every build. Release archives are tracked in
[xila-project/core#762](https://github.com/xila-project/core/issues/762).

## Nanobom

To publish vulnerability reports, create a Nanobom project for the repository,
set the Actions secret `NANOBOM_TOKEN` to its upload token, and set the Actions
variable `NANOBOM_ENABLED` to `true`. Use that project's public badge URL in
this README.

## License

The template uses the [Apache License 2.0](license.md). If you redistribute a
binary linked with picolibc, include the SDK's `COPYING.picolibc`. See Core's SDK
documentation for the SDK and compiler runtime license details.
[Xila native SDK](https://github.com/xila-project/core/blob/main/libraries/abi/application/sdk/README.md)

<!-- xila:footer:start -->

---

Part of **[Xila](https://www.xila.dev/)** ·
[Repositories](https://github.com/xila-project) ·
[Community](https://matrix.to/#/#xila:anneraud.fr)

Special thanks to [Martin Legand](https://www.behance.net/martinlegand) for generously designing the Xila logo.

<!-- xila:footer:end -->
