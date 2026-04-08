#pragma once
#include <string>

class Shader
{
public:
    Shader(const std::string &name);
    ~Shader();

    void Use(); // Activates shader
    void SetInt(const std::string &name, int value) const; // Specifies value of uniform variable

private:
    unsigned int m_ID; // Shader ID

    std::string ReadFile(const std::string &path);               // Reads shader file
    unsigned int Compile(unsigned int type, const char *source); // Compiles shader
};
