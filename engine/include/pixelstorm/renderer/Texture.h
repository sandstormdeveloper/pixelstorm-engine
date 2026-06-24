#pragma once

#include <string>

class Texture
{
public:
    Texture();
    explicit Texture(const std::string &path);
    ~Texture();

    void Bind(unsigned int slot = 0) const; // Activates and binds texture
    int GetWidth() const;                   // Returns texture width in pixels
    int GetHeight() const;                  // Returns texture height in pixels

private:
    unsigned int m_ID; // Texture ID
    int m_Width;       // Texture width in pixels
    int m_Height;      // Texture height in pixels
};
