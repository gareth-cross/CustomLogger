cmake_minimum_required(VERSION 3.11)
project(CustomLoggerThirdParty)
set(CMAKE_COLOR_MAKEFILE ON)

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

set(BUILD_SHARED OFF CACHE BOOL "" FORCE)       # Turns off /GL and /LTCG
set(WITH_LIBSODIUM OFF CACHE BOOL "" FORCE)     # Don't have libsodium, and don't need it.
SET(BUILD_TESTING OFF CACHE BOOL "" FORCE)      # Turn off libzmq tests
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE) # Turn off cppzmq tests
set(FMT_TEST OFF CACHE BOOL "" FORCE)           # Turn off libfmt tests
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)    # Turn off JSON tests

add_subdirectory(libzmq)
add_subdirectory(cppzmq)
add_subdirectory(fmt EXCLUDE_FROM_ALL)   # Don't need to install.
add_subdirectory(json)

# Add our own custom tools to the build:
add_subdirectory(tools)
