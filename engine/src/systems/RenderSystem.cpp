#include "pixelstorm/systems/RenderSystem.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/renderer/Renderer.h"
#include "pixelstorm/renderer/Shader.h"
#include "pixelstorm/renderer/Texture.h"

void RenderSystem::Render(Registry &registry, Renderer &renderer, Shader &shader)
{
    // Loops through entities in registry with a transform and sprite renderer
    for (Entity entity : registry.GetEntitiesWith<Transform, SpriteRenderer>())
    {
        // Gets components
        Transform &transform = registry.GetComponent<Transform>(entity);
        SpriteRenderer &sprite = registry.GetComponent<SpriteRenderer>(entity);

        // Draws entity
        if (sprite.Visible && sprite.TextureResource)
        {
            sprite.TextureResource->Bind();
            shader.SetVec4("u_Color", sprite.Color);
            renderer.DrawQuad(shader, transform.GetMatrix());
        }
    }
}
