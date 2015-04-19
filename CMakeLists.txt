cmake_minimum_required(VERSION 2.6)
project(pong)

set(PONG_VERSION 1.2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules ${CMAKE_MODULE_PATH})

# User options
option(ENABLE_NLS "Build with native language support" ON)
option(STATIC_LINK "Link statically" OFF)

# Build flags, platform checks and configure file
if(STATIC_LINK)
  set(CMAKE_EXE_LINKER_FLAGS
  "-static-libgcc -static-libstdc++ -static ${CMAKE_EXE_LINKER_FLAGS}")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++11 USE_FLAG_STDCXX11)

if(USE_FLAG_STDCXX11)
  add_definitions(-std=c++11)
endif()

include(CheckCXXSourceCompiles)

if(USE_FLAG_STDCXX11)
  set(CMAKE_REQUIRED_FLAGS -std=c++11)
endif()
check_cxx_source_compiles("
#include <string>
int main() {
  std::to_string(0);
  return 0;
}
" HAVE_STD_TO_STRING)

include(GNUInstallDirs)

configure_file(${CMAKE_SOURCE_DIR}/config.hpp.in
               ${CMAKE_BINARY_DIR}/config.hpp)

include_directories(${CMAKE_BINARY_DIR})

# Subdirectories
add_subdirectory(src)

if(ENABLE_NLS)
  add_subdirectory(po)
endif()
