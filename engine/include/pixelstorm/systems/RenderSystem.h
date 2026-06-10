#pragma once

class Registry;
class ResourceManager;
class Renderer;
class Shader;
class Texture;

class RenderSystem
{
public:
    void Render(Registry &registry, ResourceManager &resourceManager, Renderer &renderer, Shader &shader, Texture *fallbackTexture = nullptr, bool debugDrawColliders = false); // Renders entities
};
