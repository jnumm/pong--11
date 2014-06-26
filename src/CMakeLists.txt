cmake_minimum_required(VERSION 2.6)
project(pong)

set(pong_SRCS
    ball.cpp
    ball.hpp
    direction.hpp
    game.cpp
    game.hpp
    main.cpp
    paddle.cpp
    paddle.hpp
    prng_user.cpp
    prng_user.hpp)

if(MINGW)
  set(pong_SRCS ${pong_SRCS} to_string_replacement.hpp)
  find_package(Boost 1.20.0 REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(SFML 2 REQUIRED system window graphics)
include_directories(${SFML_INCLUDE_DIR})

add_executable(pong WIN32 ${pong_SRCS})
target_link_libraries(pong ${SFML_LIBRARIES})

install(TARGETS pong DESTINATION ${CMAKE_INSTALL_BINDIR})
