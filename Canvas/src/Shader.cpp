#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <glew.h>

Shader::Shader(const std::string& filepath)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    Bind();
    glUniform4f(GetUniformLocation(name), f0, f1, f2, f3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& mat)
{
    Bind();
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetUniformArray1i(const std::string& name, int* values, uint32_t count)
{
    Bind();
    glUniform1iv(GetUniformLocation(name), count, values);
}

void Shader::SetUniform1i(const std::string& name, int i0)
{
    Bind();
    glUniform1i(GetUniformLocation(name), i0);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocations.find(name) == m_UniformLocations.end())
    {
        int location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            std::cout << "Uniform not found of name " << name << std::endl;
        else
            m_UniformLocations[name] = location;
        return location;
    }

    return m_UniformLocations[name];
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::string openglPath = "../Canvas/";
    openglPath += filePath;

    std::ifstream stream(openglPath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
            ss[(int) type] << line << '\n';
    }

    return { ss[0].str(), ss[1].str() };
}

uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    uint32_t fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

uint32_t Shader::CompileShader(const std::string& source, uint32_t type)
{
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* msg = (char*) alloca(length);
        glGetShaderInfoLog(id, length, &length, msg);

        std::cout << "failed to compile shader" << std::endl;
        std::cout << msg << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}
