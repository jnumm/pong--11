cmake_minimum_required(VERSION 2.6)
project(pong)

set(PONG_VERSION 1.2)

set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake_modules  ${CMAKE_MODULE_PATH})

include(GNUInstallDirs)

# User options
option(ENABLE_NLS "Build with native language support" ON)

# Link mingw support libraries statically by default.
if(MINGW)
  set(CMAKE_EXE_LINKER_FLAGS
      "-static-libgcc -static-libstdc++ ${CMAKE_EXE_LINKER_FLAGS}")
endif()

# Platform checks and configure file
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_definitions(-std=c++11)
endif()

include(CheckCXXSourceCompiles)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_REQUIRED_FLAGS -std=c++11)
endif()

check_cxx_source_compiles("
#include <string>
int main() {
  std::to_string(0);
  return 0; }
" HAVE_STD_TO_STRING)

configure_file(${CMAKE_SOURCE_DIR}/config.hpp.in
               ${CMAKE_BINARY_DIR}/config.hpp)

include_directories(${CMAKE_BINARY_DIR})

# Subdirectories
add_subdirectory(src)

if(ENABLE_NLS)
  add_subdirectory(po)
endif()
