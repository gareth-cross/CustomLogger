set(CMAKE_SYSTEM_NAME Linux)

SET(UE_LINUX_TOOLCHAIN "C:/UnrealToolchains/v19_clang-11.0.1-centos7/x86_64-unknown-linux-gnu")

set(CMAKE_SYSROOT "${UE_LINUX_TOOLCHAIN}/usr")
set(CMAKE_C_COMPILER "${UE_LINUX_TOOLCHAIN}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${UE_LINUX_TOOLCHAIN}/bin/clang++.exe")
set(CMAKE_REQUIRED_INCLUDES "${UE_LINUX_TOOLCHAIN}/usr/include")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
