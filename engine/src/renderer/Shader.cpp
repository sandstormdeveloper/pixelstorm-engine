#include "pixelstorm/renderer/Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

#define SHADER_PATH "assets/shaders/"

Shader::Shader(const std::string& name)
{
    std::string vertexPath   = std::string(SHADER_PATH) + name + ".vert";
    std::string fragmentPath = std::string(SHADER_PATH) + name + ".frag";

    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty())
    {
        std::cout << "ERROR: Shader not found: " << name << std::endl;
    }

    unsigned int vertex = Compile(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fragment = Compile(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Use()
{
    glUseProgram(m_ID);
}

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "ERROR: Could not open: " << path << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::Compile(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}