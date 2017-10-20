# Defines CMAKE_INSTALL_FULL_LOCALEDIR for config.hpp
include(GNUInstallDirs)

set(SFML_STATIC_LIBRARIES "${STATIC_LINK}")
find_package(SFML 2 REQUIRED graphics window system)

find_package(Boost REQUIRED)

configure_file(config.hpp.in config.hpp)

add_executable(pong
  ball.cpp ball.hpp
  game.cpp game.hpp
  i18n.hpp
  main.cpp
  paddle.cpp paddle.hpp
  random.hpp
  pong.exe.manifest pong.rc)

set_target_properties(pong PROPERTIES
  CXX_STANDARD 17
  CXX_EXTENSIONS OFF
  WIN32_EXECUTABLE ON
  INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_BINARY_DIR};${SFML_INCLUDE_DIR};${Boost_INCLUDE_DIRS}"
  LINK_LIBRARIES "${SFML_LIBRARIES};${SFML_DEPENDENCIES}")

if(WIN32)
  target_compile_definitions(pong PRIVATE VC_EXTRALEAN WIN32_LEAN_AND_MEAN)
endif()

if(MSVC)
  target_compile_options(pong PRIVATE /MP /W4 /std:c++17 /permissive- /fp:fast /GL)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    target_compile_options(pong PRIVATE /favor:INTEL64)
  endif()
  # No manifest generation since we have one already.
  set_property(TARGET pong PROPERTY LINK_FLAGS "/ENTRY:mainCRTStartup /MANIFEST:NO /LTCG")
else()
  target_compile_options(pong PRIVATE -Wall -Wextra -pedantic -ffast-math)
  set_property(TARGET pong PROPERTY LINK_FLAGS "-Wl,--as-needed")
  if(STATIC_LINK)
    set_property(TARGET pong APPEND_STRING PROPERTY LINK_FLAGS
      " -static-libgcc -static-libstdc++ -static")
  endif()
endif()

install(TARGETS pong DESTINATION ${CMAKE_INSTALL_BINDIR})
