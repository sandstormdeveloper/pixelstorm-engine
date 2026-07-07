# Engine

This folder contains the reusable engine library that powers the demo.

## What It Provides

- Core application lifecycle and window management
- Input, timing, logging, and utility math/types
- 2D rendering, textures, shaders, font/text drawing, and camera control
- Entity/component helpers, scenes, and scene management
- Physics, particle, and animation systems
- A default CRT-style post-process pass

## Layout

- `include/pixelstorm/`: public headers
- `src/`: engine implementation
- `assets/`: built-in shaders and the default font used by the demo

## Public Entry Point

Most users include:

```cpp
#include "pixelstorm/PixelStorm.h"
```

That header re-exports the main engine types used by the demo and by game code.
