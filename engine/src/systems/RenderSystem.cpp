#include "pixelstorm/systems/RenderSystem.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/renderer/Renderer.h"
#include "pixelstorm/renderer/Shader.h"
#include "pixelstorm/renderer/Texture.h"
#include "pixelstorm/resources/ResourceManager.h"

void RenderSystem::Render(Registry &registry, ResourceManager &resourceManager, Renderer &renderer, Shader &shader, Texture *fallbackTexture)
{
    // Loops through entities in registry with a transform and sprite renderer
    for (Entity entity : registry.GetEntitiesWith<Transform, SpriteRenderer>())
    {
        // Gets components
        Transform &transform = registry.GetComponent<Transform>(entity);
        SpriteRenderer &sprite = registry.GetComponent<SpriteRenderer>(entity);

        // Resolves the named texture and falls back to the default texture if needed
        Texture *texture = fallbackTexture;
        if (!sprite.TextureName.empty())
        {
            if (Texture *loadedTexture = resourceManager.GetTexture(sprite.TextureName))
            {
                texture = loadedTexture;
            }
        }

        // Draws entity if valid
        if (sprite.Visible && texture)
        {
            texture->Bind();
            shader.SetVec4("u_Color", sprite.Color);
            renderer.DrawQuad(shader, transform.GetMatrix());
        }
    }
}
