#include "pixelstorm/core/Log.h"
#include "pixelstorm/renderer/Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

// Shader directory
#define SHADER_PATH "assets/shaders/"

Shader::Shader(const std::string &name)
{
    // Builds paths
    std::string vertexPath = std::string(SHADER_PATH) + name + ".vert";
    std::string fragmentPath = std::string(SHADER_PATH) + name + ".frag";

    // Reads vertex and fragment shaders
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    // Throws error if shaders not found
    if (vertexCode.empty() || fragmentCode.empty())
    {
        Log::Error("Shader not found: " + name);
    }

    // Compiles shaders
    unsigned int vertex = Compile(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fragment = Compile(GL_FRAGMENT_SHADER, fragmentCode.c_str());

    // Creates program in CPU
    m_ID = glCreateProgram();

    // Attaches shaders to program
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);

    // Links shaders
    glLinkProgram(m_ID);

    // Deletes temp shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    // Deletes program
    glDeleteProgram(m_ID);
}

void Shader::Use()
{
    // Activates shader
    glUseProgram(m_ID);
}

void Shader::SetInt(const std::string &name, int value) const
{
    // Specifies value of uniform variable
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const
{
    // Specifies value of mat4 uniform variable
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
    // Specifies value of vec4 uniform variable
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

std::string Shader::ReadFile(const std::string &path)
{
    // Opens file
    std::ifstream file(path);

    // Throws error if it doesn't exist
    if (!file.is_open())
    {
        Log::Error("Could not open shader file: " + path);
        return "";
    }

    // Reads the file
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::Compile(unsigned int type, const char *source)
{
    // Creates shader
    unsigned int shader = glCreateShader(type);

    // Sets source code
    glShaderSource(shader, 1, &source, nullptr);

    // Compiles shader
    glCompileShader(shader);
    return shader;
}
