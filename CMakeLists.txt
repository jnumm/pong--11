cmake_minimum_required(VERSION 2.6)
project(pong)

set(PONG_VERSION 1.2)
message(STATUS "Preparing to build ${PROJECT_NAME} v${PONG_VERSION}")

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${SFML_ROOT}/cmake/Modules
    /usr/share/SFML/cmake/Modules
)

# User options
option(ENABLE_NLS "Build with native language support" ON)
option(STATIC_LINK "Link statically" OFF)

if(ENABLE_NLS)
  message(STATUS "Building with native language support")
else()
  message(STATUS "Building without native language support")
endif()
if(STATIC_LINK)
  message(STATUS "Linking statically")
else()
  message(STATUS "Linking dynamically")
endif()

# Build flags, platform checks and configure file
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic -ffast-math ${CMAKE_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_EXE_LINKER_FLAGS}")

  if(STATIC_LINK)
    set(CMAKE_EXE_LINKER_FLAGS
        "-static-libgcc -static-libstdc++ -static ${CMAKE_EXE_LINKER_FLAGS}")
  endif()
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "/fp:fast ${CMAKE_CXX_FLAGS}")
endif()

include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
#include <string>
int main() {
  std::to_string(0);
  return 0;
}
" HAVE_STD_TO_STRING)

# Defines CMAKE_INSTALL_FULL_LOCALEDIR for config.hpp
include(GNUInstallDirs)

configure_file(${CMAKE_SOURCE_DIR}/config.hpp.in
               ${CMAKE_BINARY_DIR}/config.hpp)
include_directories(${CMAKE_BINARY_DIR})

# Subdirectories
add_subdirectory(src)

if(ENABLE_NLS)
  add_subdirectory(po)
endif()
