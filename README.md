<h1>
<picture>
  <source media="(prefers-color-scheme: dark)" srcset=".github/brand/header-dark.svg">
  <img src=".github/brand/header-light.svg" alt="Xila Native C app template — A C starter for your first native Xila application." width="1280">
</picture>
</h1>

[Documentation](https://www.xila.dev/docs/) ·
[Community](https://matrix.to/#/#xila:anneraud.fr)

Build a small C application for the Xila embedded OS. The starter prints a
greeting and its command-line arguments to stdout. The greeting names the
`USER` environment variable, then `NAME`, and falls back to `world`. Pass `--fail`
as the first argument to print a diagnostic to stderr and return status 1.
Otherwise the app returns status 0.

The example reads the environment with plain `getenv`; `setenv` and `unsetenv`
work the same way. Include `<stdlib.h>` from the SDK, whose `getenv` declaration
keeps the compiler from caching `environ`. `environ` itself is opt-in: a program
that never references it pays no RAM for it. See the
[Xila native SDK documentation](https://github.com/xila-project/core/blob/main/libraries/abi/application/sdk/README.md)
for the environment, `getpid` and `getppid`, and the `argv[0]` launch name.

## Requirements

The host must run Linux: the `sdk:build` task uses `flock`, `mv -T` and GNU tar.
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
multilib. Instead of installing the packages by hand, run
`mise run sdk:build --install-packages` to let the pinned Core checkout's
`ci:packages-sdk*` task for the selected target install them with `sudo` and
`apt-get`. CI does the same.

The first SDK build needs network access for the pinned Core source, the mise
and uv tools, and Core's pinned picolibc source. Keep the caches and the checkout
under `target/core/` to rebuild offline after the first build.

## Create your application

1. Select **Use this template** on GitHub and clone your new repository, or
   clone this repository directly.
2. Change `app_name` in `mise.toml` to the name of your executable.
3. Edit `src/main.c` to implement your application.
4. Make the repository your own:
   - Replace the header at the top of this README and delete `.github/brand/`,
     which holds the Xila template header assets and their notice.
   - Replace the "Part of Xila" footer at the bottom of this README.
   - Replace the copyright holder in `LICENSE`, or choose another license.
   - Rewrite the License section below to match.

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
`target/sdk/<target>/current/build/<app_name>.elf`. Rerun `build` and `check`
after editing the C source.

CMake also writes `compile_commands.json` into the build directory,
`target/sdk/<target>/current/build/`, which no editor finds on its own. To make
clangd use it, link it into the repository root; Git already ignores that name.

```sh
ln -s target/sdk/x86_64-unknown-linux-gnu/current/build/compile_commands.json compile_commands.json
```

The link follows the `current` SDK, so it keeps working across SDK updates for
that target. Link another target's file to get its flags instead.

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
    XILA_SDK_TARGET="$target" mise run check || break
done
git add mise.toml
git commit -m "Update the Xila Core SDK pin"
```

`sdk:build` owns SDK acquisition and stages the toolchain and checker at the
same paths on every build. Release archives are tracked in
[xila-project/core#762](https://github.com/xila-project/core/issues/762).

Each pin leaves a checkout under `target/core/<sha>` and a staging directory
`target/sdk/<target>/sdk.*` for every SDK build. Once you no longer need an
older pin, you can delete its `target/core/<sha>` checkout and the `sdk.*`
directories that `current` no longer points to, or the whole `target/`
directory. The next `sdk:build` fetches what it needs again.

`sdk:build` runs Core's own `mise run sdk:build` with mise's configuration search
stopped at this repository, so this repository's `mise.toml` and any
`[env]` or `[tools]` you add to it cannot change Core's build. Your global mise
configuration still applies.

## Nanobom

To publish vulnerability reports, create a Nanobom project for the repository,
set the Actions secret `NANOBOM_TOKEN` to its upload token, and set the Actions
variable `NANOBOM_ENABLED` to `true`. Use that project's public badge URL in
this README.

The reusable workflow uploads to Xila's Nanobom server and organization by
default, so a token from your own server fails there. Set the Actions variables
`NANOBOM_URL` to your Nanobom server's URL and `NANOBOM_ORGANIZATION` to your
organization's slug. The project name defaults to the lowercased repository
name.

## License

This template is released under the [MIT License](LICENSE), except the
`.github/brand/` header assets, which are covered by their own notice.
Applications created from it may use any license. If you redistribute a binary
linked with picolibc, include the SDK's `COPYING.picolibc`; the
[Xila native SDK documentation](https://github.com/xila-project/core/blob/main/libraries/abi/application/sdk/README.md)
gives the SDK and compiler runtime license details.

<!-- xila:footer:start -->

---

Part of **[Xila](https://www.xila.dev/)** ·
[Repositories](https://github.com/xila-project) ·
[Community](https://matrix.to/#/#xila:anneraud.fr)

Special thanks to [Martin Legand](https://www.behance.net/martinlegand) for generously designing the Xila logo.

<!-- xila:footer:end -->
