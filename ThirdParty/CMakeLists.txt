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


# set(EXE_NAME "listen")
# FILE(GLOB ${EXE_NAME}_SOURCES "source/*.cc")
# add_executable(${EXE_NAME} ${${EXE_NAME}_SOURCES})

# add_dependencies(${EXE_NAME} fmt libzmq-static cppzmq-static nlohmann_json)
# target_link_libraries(${EXE_NAME} fmt libzmq-static cppzmq-static nlohmann_json)

# # Turn on C++14
# target_compile_features(${EXE_NAME} PUBLIC cxx_std_14)

# # turn on maximum warnings
# if (MSVC)
#   target_compile_options(${EXE_NAME} PRIVATE /W4 /WX /wd4244)
# else ()
#   target_compile_options(${EXE_NAME} PRIVATE
#       -Wall -Wextra -pedantic -Werror -Wno-sign-compare)
# endif ()
