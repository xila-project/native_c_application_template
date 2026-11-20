# Xila native C application template

[Documentation](https://www.xila.dev/docs/) ·
[Try Xila](https://platforms.xila.dev/) ·
[Community](https://matrix.to/#/#xila:anneraud.fr)

Build a small C application for the Xila embedded OS. The starter prints a
greeting, lists its arguments, and reads `XILA_GREETING` from the Xila process
environment. It returns status 0 by default and status 1 when the first argument
is `--fail`.

## Requirements

- Git, [mise](https://mise.jdx.dev/), Clang, LLD, LLVM tools (`llvm-ar`), CMake
  3.20 or newer, Make, GNU tar, and zstd.
- Network access for the Core submodule, mise tools, and Core's pinned picolibc
  source. The SDK build uses Python 3.12 or newer and runs Meson and Ninja
  through uv.
- For Cortex-M4F, install `gcc-arm-none-eabi`. For RV32IMAC, install
  `gcc-riscv64-unknown-elf` with the `rv32imac` / `ilp32` multilib. These
  toolchains supply the compiler runtime (`libgcc.a`).

## Quick start

Use **Use this template** on GitHub or clone the repository directly. In your
checkout, initialize Core and trust the two mise configurations after reviewing
them.

```sh
git submodule update --init
mise trust
(cd core && mise trust)
mise run sdk:build
mise run build
mise run check
```

The default target is Linux x86-64. The SDK is unpacked in
`target/sdk/<target>/` and the application is written to
`target/<target>/hello.elf`. Edit `src/main.c` to customize the example. After
editing the C source, rerun `mise run build` and `mise run check`.

Set `XILA_SDK_TARGET` for all three tasks to build for a different platform.

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

## Run on Xila

Use a Xila build for the same target that enables Core's `native-dev` feature
and registers `NativeFormat`. Copy `hello.elf`, a plain static-PIE ELF, to a
filesystem mounted by that build and give it executable permissions in Xila's
VFS. From a Xila shell on that build, invoke its path with arguments, for example
`/hello.elf world`. Launch it with `XILA_GREETING` in the Xila process
environment to print its value. The example uses a 64-byte buffer and accepts
values up to 63 bytes.

On Linux x86-64, use the same Xila build and VFS procedure. For a headless host
test, Core's SDK test shows how to register `NativeFormat`, write the ELF into
the test VFS, mark it executable and launch it with `execute_with`. Use
`Launch::with_environment` to supply the test environment and `join` to read the
exit status. [Core SDK host test](https://github.com/xila-project/core/blob/main/libraries/executable/tests/sdk.rs)

## Update the SDK pin

With a clean Core submodule, fetch Core's `main`, check out the reviewed revision
and record the new gitlink in this repository.

```sh
git -C core fetch origin main
git -C core checkout --detach FETCH_HEAD
git add core
```

Rebuild the SDK and rebuild and check the app for all three targets before
committing the pin. Until SDK archives are published as release assets,
`sdk:build` builds from Core and stages the toolchain and checker. That task
owns SDK acquisition, so the application build and check use the same paths
when release downloads become available. [SDK release tracking issue](https://github.com/xila-project/core/issues/762)

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
