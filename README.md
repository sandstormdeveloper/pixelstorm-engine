# PixelStorm Engine

PixelStorm is a small **2D game engine** focused on **pixel-art** projects. It is built in **C++17** on top of **OpenGL** and ships with a playable demo.

## Current State

- The project builds with **CMake** into a static engine library plus a demo executable.
- The engine includes rendering, input, scenes, ECS-style entities, physics, particles, text, and a CRT-style post-process pass.
- The demo currently includes a menu, a simple top-down shooter, and a game-over scene.
- A Debug build was verified in this workspace.

## Build

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## Run

After building, run the demo from the output folder:

```powershell
build/bin/Debug/pixelstorm_demo.exe
```

## Notes

- Runtime assets are copied next to the demo executable after the build.
- The code base uses **GLAD**, **GLFW**, **GLM**, and **CMake**.
