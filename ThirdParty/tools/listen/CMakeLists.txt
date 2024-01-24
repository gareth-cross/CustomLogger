cmake_minimum_required(VERSION 3.11)
project(CustomLoggerListenTool)
set(CMAKE_COLOR_MAKEFILE ON)

# Single binary target:
set(EXE_NAME "listen")
add_executable(${EXE_NAME} source/json_decoding.cc source/main.cc)

add_dependencies(${EXE_NAME} fmt libzmq-static cppzmq-static nlohmann_json)
target_link_libraries(${EXE_NAME} fmt libzmq-static cppzmq-static nlohmann_json)

# Turn on C++14
target_compile_features(${EXE_NAME} PUBLIC cxx_std_14)

# turn on maximum warnings
if (MSVC)
  target_compile_options(${EXE_NAME} PRIVATE /W4 /WX /wd4244)
else ()
  target_compile_options(${EXE_NAME} PRIVATE
      -Wall -Wextra -pedantic -Werror -Wno-sign-compare)
endif ()

# make cache variables for install destinations
include(GNUInstallDirs)

# Setup install target
install(
    TARGETS ${EXE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
