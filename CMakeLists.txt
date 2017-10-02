cmake_minimum_required(VERSION 3.1)
project(pong LANGUAGES CXX)

set(PONG_VERSION 1.6)
set(PONG_VERSION_FOURPART ${PONG_VERSION}.0.0)
string(REPLACE "." "," PONG_VERSION_COMMA ${PONG_VERSION_FOURPART})

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
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WIN32_EXECUTABLE ON)

if(WIN32)
  add_definitions(-DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -pedantic -ffast-math)
  set(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_EXE_LINKER_FLAGS}")

  if(STATIC_LINK)
    set(CMAKE_EXE_LINKER_FLAGS
        "-static-libgcc -static-libstdc++ -static ${CMAKE_EXE_LINKER_FLAGS}")
  endif()
elseif(MSVC)
  add_compile_options(/MP /fp:fast)
endif()

# Defines CMAKE_INSTALL_FULL_LOCALEDIR for config.hpp
include(GNUInstallDirs)

configure_file(config.hpp.in config.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Subdirectories
add_subdirectory(src)

if(ENABLE_NLS)
  add_subdirectory(po)
endif()
