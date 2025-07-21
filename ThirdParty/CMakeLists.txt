set(BUILD_SHARED OFF)

function(add_libzmq)
  if(NOT TARGET libzmq-static)
    set(ENABLE_CPACK
        OFF
        CACHE BOOL "" FORCE)
    set(ZMQ_BUILD_TESTS
        OFF
        CACHE BOOL "" FORCE)
    set(WITH_LIBSODIUM
        OFF
        CACHE BOOL "" FORCE)
    if(NOT MSVC)
      # No pthreads on linux.
      set(ZMQ_CV_IMPL
          "none"
          CACHE STRING "" FORCE)
    endif()
    add_subdirectory(libzmq)
  endif()
endfunction()
add_libzmq()

function(add_cppzmq)
  if(NOT TARGET cppzmq-static)
    set(CPPZMQ_BUILD_TESTS
        OFF
        CACHE BOOL "" FORCE)
    add_subdirectory(cppzmq)
  endif()
endfunction()
add_cppzmq()

function(add_json)
  if(NOT TARGET nlohmann_json)
    add_subdirectory(json EXCLUDE_FROM_ALL)
  endif()
endfunction()
add_json()

function(add_fmt)
  if(NOT TARGET fmt-header-only)
    set(FMT_TEST
        OFF
        CACHE BOOL "" FORCE)
    add_subdirectory(fmt EXCLUDE_FROM_ALL)
  endif()
endfunction()
add_fmt()
