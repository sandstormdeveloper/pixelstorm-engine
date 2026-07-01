#include "pixelstorm/resources/AssetLoader.h"
#include "pixelstorm/renderer/Font.h"
#include "pixelstorm/renderer/Texture.h"

std::unique_ptr<Texture> AssetLoader::LoadTexture(const std::string &path) 
{
    // Creates texture from file path
    return std::make_unique<Texture>(path);
}

std::unique_ptr<Font> AssetLoader::LoadFont(const std::string &path, float pixelHeight)
{
    // Creates font from file path
    return std::make_unique<Font>(path, pixelHeight);
}
