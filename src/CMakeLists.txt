# Defines CMAKE_INSTALL_FULL_LOCALEDIR for config.hpp
include(GNUInstallDirs)

set(SFML_STATIC_LIBRARIES "${STATIC_LINK}")
find_package(SFML 2 REQUIRED graphics window system)

configure_file(config.hpp.in config.hpp)

add_executable(pong
  ball.hpp
  game.cpp game.hpp
  i18n.hpp
  main.cpp
  paddle.cpp paddle.hpp
  random.cpp random.hpp
  pong.rc)

set_target_properties(pong PROPERTIES
  CXX_STANDARD 11
  CXX_EXTENSIONS OFF
  WIN32_EXECUTABLE ON
  COMPILE_OPTIONS
    "$<IF:$<CXX_COMPILER_ID:MSVC>,/MP;/fp:fast,-Wall;-Wextra;-pedantic;-ffast-math>"
  COMPILE_DEFINITIONS
    "$<$<PLATFORM_ID:Windows>:VC_EXTRALEAN;WIN32_LEAN_AND_MEAN>"
  INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR};${SFML_INCLUDE_DIR}"
  LINK_LIBRARIES "${SFML_LIBRARIES};${SFML_DEPENDENCIES}")

# LINK_FLAGS are different since they do not support generator expressions
if(MSVC)
  # Enables Windows “Visual Styles”, i.e. native look and feel
  set_property(TARGET pong PROPERTY LINK_FLAGS
    "/ENTRY:mainCRTStartup \
     /MANIFESTDEPENDENCY:\"type='win32' \
                           name='Microsoft.Windows.Common-Controls' \
                           version='6.0.0.0' \
                           processorArchitecture='*' \
                           publicKeyToken='6595b64144ccf1df' \
                           language='*'\"")
else()
  set_property(TARGET pong PROPERTY LINK_FLAGS "-Wl,--as-needed")
  if(STATIC_LINK)
    set_property(TARGET pong APPEND_STRING PROPERTY LINK_FLAGS
      " -static-libgcc -static-libstdc++ -static ")
  endif()
endif()

if(WIN32 AND NOT MSVC)
  configure_file(pong.exe.manifest.in pong.exe.manifest)
endif()

install(TARGETS pong DESTINATION ${CMAKE_INSTALL_BINDIR})
