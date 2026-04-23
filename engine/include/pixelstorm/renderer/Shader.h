#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <string>

class Shader
{
public:
    Shader(const std::string &name);
    ~Shader();

    void Use();                                                          // Activates shader
    void SetInt(const std::string &name, int value) const;               // Specifies value of uniform variable
    void SetMat4(const std::string &name, const glm::mat4 &value) const; // Specifies value of mat4 uniform
    void SetVec4(const std::string &name, const glm::vec4 &value) const; // Specifies value of vec4 uniform

private:
    unsigned int m_ID; // Shader ID

    std::string ReadFile(const std::string &path);               // Reads shader file
    unsigned int Compile(unsigned int type, const char *source); // Compiles shader
};
