#include "pixelstorm/resources/ResourceManager.h"
#include "pixelstorm/resources/AssetLoader.h"
#include "pixelstorm/renderer/Font.h"
#include "pixelstorm/renderer/Texture.h"

bool ResourceManager::LoadTexture(const std::string &name, const std::string &path)
{
    // Reuses texture if it already exists
    if (HasTexture(name))
    {
        return true;
    }

    // Loads texture from file
    std::unique_ptr<Texture> texture = AssetLoader::LoadTexture(path);
    if (!texture)
    {
        return false;
    }

    // Stores loaded texture by name
    m_Textures[name] = std::move(texture);
    return true;
}

bool ResourceManager::LoadFont(const std::string &name, const std::string &path, float pixelHeight)
{
    // Reuses font if it already exists
    if (HasFont(name))
    {
        return true;
    }

    // Loads font from file
    std::unique_ptr<Font> font = AssetLoader::LoadFont(path, pixelHeight);
    if (!font || !font->IsValid())
    {
        return false;
    }

    // Stores loaded font by name
    m_Fonts[name] = std::move(font);
    return true;
}

bool ResourceManager::HasTexture(const std::string &name) const
{
    // Returns if texture exists in storage
    return m_Textures.find(name) != m_Textures.end();
}

Texture *ResourceManager::GetTexture(const std::string &name)
{
    // Gets texture iterator
    std::unordered_map<std::string, std::unique_ptr<Texture>>::iterator iterator = m_Textures.find(name);
    if (iterator == m_Textures.end())
    {
        return nullptr;
    }

    // Returns stored texture
    return iterator->second.get();
}

const Texture *ResourceManager::GetTexture(const std::string &name) const
{
    // Gets texture iterator in read-only mode
    std::unordered_map<std::string, std::unique_ptr<Texture>>::const_iterator iterator = m_Textures.find(name);
    if (iterator == m_Textures.end())
    {
        return nullptr;
    }

    // Returns stored texture
    return iterator->second.get();
}

bool ResourceManager::HasFont(const std::string &name) const
{
    // Returns if font exists in storage
    return m_Fonts.find(name) != m_Fonts.end();
}

Font *ResourceManager::GetFont(const std::string &name)
{
    // Gets font iterator
    std::unordered_map<std::string, std::unique_ptr<Font>>::iterator iterator = m_Fonts.find(name);
    if (iterator == m_Fonts.end())
    {
        return nullptr;
    }

    // Returns stored font
    return iterator->second.get();
}

const Font *ResourceManager::GetFont(const std::string &name) const
{
    // Gets font iterator in read-only mode
    std::unordered_map<std::string, std::unique_ptr<Font>>::const_iterator iterator = m_Fonts.find(name);
    if (iterator == m_Fonts.end())
    {
        return nullptr;
    }

    // Returns stored font
    return iterator->second.get();
}

void ResourceManager::Clear()
{
    // Clears stored textures
    m_Textures.clear();

    // Clears stored fonts
    m_Fonts.clear();
}
