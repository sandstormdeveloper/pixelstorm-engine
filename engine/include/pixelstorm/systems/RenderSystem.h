#pragma once

class Registry;
class Renderer;
class Shader;

class RenderSystem
{
public:
    void Render(Registry &registry, Renderer &renderer, Shader &shader);
};