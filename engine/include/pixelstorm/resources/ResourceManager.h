#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Texture;
class Font;

class ResourceManager
{
public:
    bool LoadTexture(const std::string &name, const std::string &path);                 // Loads texture and stores it by name
    bool LoadFont(const std::string &name, const std::string &path, float pixelHeight); // Loads font and stores it by name
    bool HasTexture(const std::string &name) const;                                     // Returns if texture exists
    Texture *GetTexture(const std::string &name);                                       // Returns stored texture
    const Texture *GetTexture(const std::string &name) const;                           // Returns stored texture in read-only mode
    bool HasFont(const std::string &name) const;                                        // Returns if font exists
    Font *GetFont(const std::string &name);                                             // Returns stored font
    const Font *GetFont(const std::string &name) const;                                 // Returns stored font in read-only mode
    void Clear();                                                                       // Removes all stored resources

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures; // Stored textures
    std::unordered_map<std::string, std::unique_ptr<Font>> m_Fonts;       // Stored fonts
};
