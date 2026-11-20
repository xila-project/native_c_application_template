cmake_minimum_required(VERSION 3.20)
project(xila_hello C)

add_executable(hello src/main.c)
target_compile_features(hello PRIVATE c_std_11)
target_link_options(hello PRIVATE --wrap=xila_main)

# The SDK toolchain links these files directly. Relink after an SDK rebuild.
set_property(TARGET hello APPEND PROPERTY LINK_DEPENDS
  "${XILA_SDK_ROOT}/lib/crt0.o"
  "${XILA_SDK_ROOT}/lib/libxila.a"
  "${XILA_SDK_ROOT}/lib/libxila_posix.a"
  "${XILA_SDK_ROOT}/lib/libc.a"
  "${XILA_SDK_ROOT}/lib/xila-static-pie.ld")
