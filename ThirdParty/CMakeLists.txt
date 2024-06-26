cmake_minimum_required(VERSION 3.11)
project(CustomLoggerThirdParty LANGUAGES CXX C)
set(CMAKE_COLOR_MAKEFILE ON)

message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

# Set the install prefix automatically when unspecified.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    # Make the install sub-directory a function of platform so we can have build binaries for both:
    if (MSVC)
        set(INSTALL_CHILD_DIRECTORY "install/Windows")
    else ()
        set(INSTALL_CHILD_DIRECTORY "install/Linux")
    endif ()

    set(CUSTOM_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/${INSTALL_CHILD_DIRECTORY}")
    message(STATUS "Changing install prefix to: ${CUSTOM_INSTALL_PREFIX}")
    set(CMAKE_INSTALL_PREFIX ${CUSTOM_INSTALL_PREFIX} CACHE PATH "Override the install prefix." FORCE)

endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Set the build-type automatically when unspecified
# See https://blog.kitware.com/cmake-and-the-default-build-type/
set(AUTO_BUILD_TYPE "RelWithDebInfo")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${AUTO_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${AUTO_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()

# Detect windows, and set the architecture.
option(OVERRIDE_ARCH_ON_WIN32 "Automatically set x64 on windows platform." ON)
if (MSVC AND ${OVERRIDE_ARCH_ON_WIN32})
    message(STATUS "Changing architecture because we are on windows.")
    set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif ()

# When building for linux, make sure clang uses libc++
if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")
endif ()

set(BUILD_SHARED OFF CACHE BOOL "" FORCE)         # Turns off building shared libzmq
set(WITH_LIBSODIUM OFF CACHE BOOL "" FORCE)       # Don't have libsodium, and don't need it.
SET(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)      # Turn off libzmq tests
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)   # Turn off cppzmq tests
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)      # Turn off JSON tests

if (NOT MSVC)
    # don't link in pthreads on linux, causes linker error when we link to UE
    set(ZMQ_CV_IMPL "none" CACHE STRING "" FORCE)
endif ()

set(ENABLE_CPACK OFF CACHE BOOL "" FORCE)    # Turn off cpack, no installer required.
add_subdirectory(libzmq)
add_subdirectory(cppzmq)
add_subdirectory(json)

if (NOT TARGET fmt-header-only)
    set(FMT_TEST OFF CACHE BOOL "" FORCE)    # Turn off libfmt tests
    add_subdirectory(fmt EXCLUDE_FROM_ALL)   # Don't need to install.
endif ()

# Add our own custom tools to the build:
add_subdirectory(tools)
