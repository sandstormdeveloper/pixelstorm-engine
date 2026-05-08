#include "pixelstorm/components/SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
    : TextureResource(nullptr),
      Color(1.0f, 1.0f, 1.0f, 1.0f),
      Visible(true)
{
}

SpriteRenderer::SpriteRenderer(const glm::vec4 &color, bool visible)
    : TextureResource(nullptr),
      Color(color),
      Visible(visible)
{
}

SpriteRenderer::SpriteRenderer(Texture *texture, const glm::vec4 &color, bool visible)
    : TextureResource(texture),
      Color(color),
      Visible(visible)
{
}
