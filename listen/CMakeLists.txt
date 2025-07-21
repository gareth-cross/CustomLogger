add_executable(listen source/json_decoding.cc source/json_decoding.h
                      source/main.cc source/colors.h)
target_link_libraries(listen fmt::fmt-header-only libzmq cppzmq
                      nlohmann_json::nlohmann_json)

if(MSVC)
  target_compile_options(listen PRIVATE /W4 /WX /wd4244)
else()
  target_compile_options(listen PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

include(GNUInstallDirs)
install(
  TARGETS listen
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
