#include "pixelstorm/systems/RenderSystem.h"
#include "pixelstorm/components/SpriteRenderer.h"
#include "pixelstorm/components/Transform.h"
#include "pixelstorm/components/Collider.h"
#include "pixelstorm/ecs/Registry.h"
#include "pixelstorm/physics/Collision.h"
#include "pixelstorm/renderer/Renderer.h"
#include "pixelstorm/renderer/Shader.h"
#include "pixelstorm/renderer/Texture.h"
#include "pixelstorm/resources/ResourceManager.h"

#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <vector>

void RenderSystem::Render(Registry &registry, ResourceManager &resourceManager, Renderer &renderer, Shader &shader, Texture *fallbackTexture, bool debugDrawColliders)
{
    // Collects renderable entities so they can be sorted before drawing
    std::vector<Entity> renderEntities = registry.GetEntitiesWith<Transform, SpriteRenderer>();

    std::stable_sort(renderEntities.begin(), renderEntities.end(),
                     [&registry](const Entity &left, const Entity &right)
                     {
                         const SpriteRenderer &leftSprite = registry.GetComponent<SpriteRenderer>(left);
                         const SpriteRenderer &rightSprite = registry.GetComponent<SpriteRenderer>(right);

                         if (leftSprite.RenderOrder != rightSprite.RenderOrder)
                         {
                             return leftSprite.RenderOrder < rightSprite.RenderOrder;
                         }

                         const Transform &leftTransform = registry.GetComponent<Transform>(left);
                         const Transform &rightTransform = registry.GetComponent<Transform>(right);

                         if (leftTransform.Position.y != rightTransform.Position.y)
                         {
                             return leftTransform.Position.y < rightTransform.Position.y;
                         }

                         return left.GetId() < right.GetId();
                     });

    shader.SetInt("u_UseTexture", 1);

    // Draws sprites in the order determined by layer and position
    for (Entity entity : renderEntities)
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
            const float textureWidth = static_cast<float>(std::max(texture->GetWidth(), 1));
            const float textureHeight = static_cast<float>(std::max(texture->GetHeight(), 1));

            // Uses the requested sprite region or the full texture if none was set
            const glm::ivec4 sourceRect = (sprite.SourceRect.z > 0 && sprite.SourceRect.w > 0)
                                              ? sprite.SourceRect
                                              : glm::ivec4(0, 0, static_cast<int>(textureWidth), static_cast<int>(textureHeight));

            // Flips the UV rectangle by moving the origin to the opposite edge
            const float sourceX = sprite.FlipX ? static_cast<float>(sourceRect.x + sourceRect.z) : static_cast<float>(sourceRect.x);
            const float sourceY = sprite.FlipY ? static_cast<float>(sourceRect.y + sourceRect.w) : static_cast<float>(sourceRect.y);
            const float sourceWidth = sprite.FlipX ? -static_cast<float>(sourceRect.z) : static_cast<float>(sourceRect.z);
            const float sourceHeight = sprite.FlipY ? -static_cast<float>(sourceRect.w) : static_cast<float>(sourceRect.w);

            // Normalizes the rectangle for the fragment shader
            const glm::vec4 normalizedSourceRect(
                sourceX / textureWidth,
                sourceY / textureHeight,
                sourceWidth / textureWidth,
                sourceHeight / textureHeight);

            texture->Bind();
            shader.SetVec4("u_Color", sprite.Color);
            shader.SetVec4("u_SourceRect", normalizedSourceRect);
            renderer.DrawQuad(shader, transform.GetMatrix());
        }
    }

    if (!debugDrawColliders)
    {
        return;
    }

    // Draws collider outlines on top of the scene for debugging
    std::vector<Entity> colliderEntities = registry.GetEntitiesWith<Transform, Collider>();
    if (fallbackTexture)
    {
        fallbackTexture->Bind();
    }

    shader.SetInt("u_Texture", 0);
    shader.SetInt("u_UseTexture", 0);
    glLineWidth(2.0f);

    for (Entity entity : colliderEntities)
    {
        const Transform &transform = registry.GetComponent<Transform>(entity);
        const Collider &collider = registry.GetComponent<Collider>(entity);
        const AABB box = Collision::CreateAABB(transform, collider);

        const glm::vec2 boxSize = box.Max - box.Min;
        const glm::vec2 boxCenter = (box.Min + box.Max) * 0.5f;
        const glm::mat4 debugModel = glm::translate(glm::mat4(1.0f), glm::vec3(boxCenter, 0.0f)) *
                                     glm::scale(glm::mat4(1.0f), glm::vec3(boxSize, 1.0f));

        shader.SetVec4("u_Color", collider.IsTrigger ? glm::vec4(0.0f, 1.0f, 0.8f, 1.0f) : glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        renderer.DrawQuadOutline(shader, debugModel);
    }

    glLineWidth(1.0f);
}
