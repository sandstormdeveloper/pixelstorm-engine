#pragma once
#include <string>

class Shader
{
public:
    Shader(const std::string& name);
    ~Shader();

    void Use();

private:
    unsigned int m_ID;

    std::string ReadFile(const std::string& path);
    unsigned int Compile(unsigned int type, const char* source);
};