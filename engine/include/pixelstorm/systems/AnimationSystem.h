#pragma once

class Registry;

class AnimationSystem
{
public:
    void Update(Registry &registry, float deltaTime); // Advances animated sprites and writes the current frame to the sprite component
};
