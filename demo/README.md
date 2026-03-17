# The Demo

This is a **demo** built to demonstrate the functionalities of the game engine.

## How to build with CMake

The following **commands** demonstrate how to **build** your game with **CMake**, and generate a working *.exe* file:

**Only Debug** (with terminal)
```console
cmake -S . -B build; cmake --build build -j 4
```

**Only Release** (without terminal)
```console
cmake -S . -B build; cmake --build build -j 4 --config Release
```

**Debug and Release**
```console
cmake -S . -B build; cmake --build build -j 4; cmake --build build -j 4 --config Release
```