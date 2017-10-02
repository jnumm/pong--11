cmake_minimum_required(VERSION 3.1)
project(pong)

set(pong_SRCS
    ball.hpp
    game.cpp
    game.hpp
    i18n.hpp
    main.cpp
    paddle.cpp
    paddle.hpp
    random.cpp
    random.hpp
    pong.rc
)

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "/ENTRY:mainCRTStartup \
  /MANIFESTDEPENDENCY:\"type='win32' \
  name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
  processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\" \
  ${CMAKE_EXE_LINKER_FLAGS}")
elseif(WIN32 AND NOT MSVC)
  configure_file(pong.exe.manifest.in pong.exe.manifest)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

if(STATIC_LINK)
  set(SFML_STATIC_LIBRARIES ON)
endif()
find_package(SFML 2 REQUIRED graphics window system)
include_directories(${SFML_INCLUDE_DIR})

add_executable(pong ${pong_SRCS})
target_link_libraries(pong ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

install(TARGETS pong DESTINATION ${CMAKE_INSTALL_BINDIR})
