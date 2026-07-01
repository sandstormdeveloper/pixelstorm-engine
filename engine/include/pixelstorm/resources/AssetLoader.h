#pragma once

#include <memory>
#include <string>

class Texture;
class Font;

class AssetLoader
{
public:
    static std::unique_ptr<Texture> LoadTexture(const std::string &path);              // Loads texture from file path
    static std::unique_ptr<Font> LoadFont(const std::string &path, float pixelHeight); // Loads font from file path
};
