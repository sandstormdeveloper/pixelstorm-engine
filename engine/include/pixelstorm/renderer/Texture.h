#pragma once

#include <string>

class Texture
{
public:
    Texture();
    explicit Texture(const std::string &path);
    ~Texture();

    void Bind(unsigned int slot = 0) const; // Activates and binds texture

private:
    unsigned int m_ID; // Texture ID
};
