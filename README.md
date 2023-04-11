# qpiskvork

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/81b307ac0e004c58adeef0c80f620310)](https://app.codacy.com/gh/Joker2770/qpiskvork?utm_source=github.com&utm_medium=referral&utm_content=Joker2770/qpiskvork&utm_campaign=Badge_Grade_Settings)

Another gomoku or renju manager adapting to Windows and Linux systems.

## summary
This program aims to become a gomoku or renju manager with the [protocol](https://plastovicka.github.io/protocl2en.htm).

## dependencies
dependencies of compiling.
* Qt5+ (REQUIRED COMPONENTS Core Gui Widgets)
* qmake or cmake

## build
- with QtCreator

Simply build with QtCreator by double-click on `qpiskvork.pro` first of all.

- with cmake toolchain

Simply build with the steps as fallow:

```shell
cd qpiskvork
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

- with xmake toolchain

More easier than cmake.

```shell
cd qpiskvork
xmake config --mode=release
xmake
```

## references

1. [https://sourceforge.net/projects/piskvork/](https://sourceforge.net/projects/piskvork/).
2. [https://plastovicka.github.io/protocl2en.htm](https://plastovicka.github.io/protocl2en.htm).
3. [https://gomocup.org/](https://gomocup.org/).
4. [https://www.renju.net/rifrules/](https://www.renju.net/rifrules/).
5. [https://www.renju.net/rule/](https://www.renju.net/rule/)
