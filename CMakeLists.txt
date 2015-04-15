cmake_minimum_required(VERSION 2.6)
project(pong)

set(PONG_VERSION 1.2)

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_definitions(-std=c++11)
endif()

# Link mingw support libraries statically by default.
if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS
      "-static-libgcc -static-libstdc++ ${CMAKE_EXE_LINKER_FLAGS}")
endif()

configure_file(${CMAKE_SOURCE_DIR}/config.hpp.in
               ${CMAKE_BINARY_DIR}/config.hpp)

include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(po)
add_subdirectory(src)
