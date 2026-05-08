#pragma once

class Registry;
class Renderer;
class Shader;
class Texture;

class RenderSystem
{
public:
    void Render(Registry &registry, Renderer &renderer, Shader &shader, Texture *fallbackTexture = nullptr); // Renders entities
};
