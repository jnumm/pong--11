cmake_minimum_required(VERSION 3.1)
project(pong VERSION 1.6 LANGUAGES CXX)

set(PROJECT_VERSION_FOURPART "${PROJECT_VERSION}.0.0")
string(REPLACE "." "," PROJECT_VERSION_COMMA "${PROJECT_VERSION_FOURPART}")

message(STATUS "Preparing to build ${PROJECT_NAME} v${PROJECT_VERSION}")

list(APPEND CMAKE_MODULE_PATH
  "${SFML_ROOT}/cmake/Modules" "/usr/share/SFML/cmake/Modules")

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

# Subdirectories
add_subdirectory(src)

if(ENABLE_NLS)
  add_subdirectory(po)
endif()
