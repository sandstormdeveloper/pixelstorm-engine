#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Texture;

class ResourceManager
{
public:
    bool LoadTexture(const std::string &name, const std::string &path); // Loads texture and stores it by name
    bool HasTexture(const std::string &name) const;                     // Returns if texture exists
    Texture *GetTexture(const std::string &name);                       // Returns stored texture
    const Texture *GetTexture(const std::string &name) const;           // Returns stored texture in read-only mode
    void Clear();                                                       // Removes all stored resources

private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures; // Stored textures
};
