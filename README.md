# pong--11 - a simple pong game

This is a simple pong game for one or two players.

It is written in C++11 using the [SFML](http://www.sfml-dev.org) library.

## help

To play in single-player mode, use the command line switch `-s`.

Player 1, in the left hand side, uses WS to move up and down.
Player 2, in the right hand side, uses up and down arrows to move.

## license

* _pong--11_ is licensed under the Apache 2.0 license. The file COPYING contains
the full license terms.

## compilation

### linux

    mkdir -p build/gcc
    (cd build/gcc && CXXFLAGS="-march=native -O2" cmake ../..)
    (cd build/gcc && make -j4)

### windows (microsoft visual studio)

Compilation is possible only with Visual Studio 14 2015 or newer.

There should be an appropriately named SFML directory. Modify "C:/Devel" as needed.

    set vcver=14
    set sfmlver=2.3.2
    mkdir build\vc%vcver%
    cd build\vc%vcver%
    cmake ../.. -G"Visual Studio %vcver%" -DSFML_ROOT="C:/Devel/SFML-%sfmlver%_vc%vcver%" -DENABLE_NLS=FALSE -DSTATIC_LINK=TRUE

### windows (mingw)

As above, an SFML directory in C:/Devel is assumed. In addition, MinGW
binaries should be in `%PATH%`.

    set gccver=5.2.0
    set sfmlver=2.3.2
    mkdir build\gcc-%gccver%
    cd build\gcc-%gccver%
    cmake ../.. -G"MinGW Makefiles" -DSFML_ROOT="C:/Devel/SFML-%sfmlver%_gcc-%gccver%" -DENABLE_NLS=FALSE -DSTATIC_LINK=TRUE
    mingw32-make
