cmake_minimum_required(VERSION 3.20)
project(xila_native_application LANGUAGES C)

if(NOT XILA_SDK_ROOT)
  message(FATAL_ERROR "Configure with the Xila SDK's xila-toolchain.cmake.")
endif()
if(NOT XILA_APP_NAME)
  message(FATAL_ERROR "Set XILA_APP_NAME to the executable name.")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable("${XILA_APP_NAME}" src/main.c)
target_compile_features("${XILA_APP_NAME}" PRIVATE c_std_11)
