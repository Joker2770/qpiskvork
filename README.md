# qpiskvork

[![Release](https://github.com/Joker2770/qpiskvork/actions/workflows/release.yml/badge.svg)](https://github.com/Joker2770/qpiskvork/actions/workflows/release.yml)
[![CMake](https://github.com/Joker2770/qpiskvork/actions/workflows/cmake.yml/badge.svg)](https://github.com/Joker2770/qpiskvork/actions/workflows/cmake.yml)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/81b307ac0e004c58adeef0c80f620310)](https://app.codacy.com/gh/Joker2770/qpiskvork?utm_source=github.com&utm_medium=referral&utm_content=Joker2770/qpiskvork&utm_campaign=Badge_Grade_Settings)
[![qpiskvork](https://snapcraft.io/qpiskvork/badge.svg)](https://snapcraft.io/qpiskvork)
[![qpiskvork](https://snapcraft.io/qpiskvork/trending.svg?name=0)](https://snapcraft.io/qpiskvork)

Another gomoku or renju manager adapting to Windows and Linux systems.

## summary

This program aims to become a gomoku or renju manager with the [protocol](https://plastovicka.github.io/protocl2en.htm).

## features

### Visualize the AI thinking (MCTS visit distribution)

When `Show -> Display AI Mind` (shortcut `A`) is enabled, while it is a computer player's turn the manager watches the engine's pipe for data groups polled in the form:

```
DEBUG thinking x1,y1,visits1 x2,y2,visits2 ...
```

Each received data group is processed as follows:

1. **Validation** - a data group is accepted only if:
   - it starts with the `DEBUG thinking` header and every item is a valid `x,y,visits` triple with non-negative integer visits;
   - the number of items does not exceed the number of free positions left on the board;
   - none of the coordinates is already occupied;
   - none of the coordinates is out of the board;
   - no coordinate is duplicated inside the group;
   - the sum of all visits in the group is greater than zero.
2. **Conversion** - the raw MCTS `visits` are turned into decision probabilities with a numerically stabilized softmax followed by normalization.
3. **Rendering** - the coordinate-to-probability data is drawn on the board as a heatmap: more likely moves get bigger, more opaque red blobs, less likely ones blue blobs, and the top candidates are labeled with their percentages.
4. **Live update** - the engine may poll new data groups while it is thinking; the display is refreshed every time a valid group is collected, until the AI actually makes its move.

## dependencies

dependencies of compiling.
* Qt5+ (REQUIRED COMPONENTS Core Gui Widgets Multimedia)
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

## features

1. Sounds effect support.

## references

1. [https://sourceforge.net/projects/piskvork/](https://sourceforge.net/projects/piskvork/).
2. [https://plastovicka.github.io/protocl2en.htm](https://plastovicka.github.io/protocl2en.htm).
3. [https://gomocup.org/](https://gomocup.org/).
4. [https://www.renju.net/rifrules/](https://www.renju.net/rifrules/).
5. [https://www.renju.net/rule/](https://www.renju.net/rule/)
