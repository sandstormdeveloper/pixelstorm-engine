# Demo

This folder contains the playable sample used to exercise the engine. It is a small top-down shooter with:

- a menu scene,
- a gameplay scene with movement, shooting, enemies, particles, and camera follow,
- a game-over scene with retry flow.

## Controls

- `WASD` or arrow keys: move
- Mouse left button: shoot
- `Space`: start / retry
- `F3`: toggle collider debug drawing

## Build

From the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

For a release build:

```powershell
cmake --build build --config Release
```

The demo executable is copied to `build/bin/<config>/pixelstorm_demo.exe`.
