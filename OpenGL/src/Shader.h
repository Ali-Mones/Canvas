#pragma once
#include <string>
#include <stdint.h>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
public:
	Shader() = default;
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniform1i(const std::string& name, int i0);
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& mat);
private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filePath);
	uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	uint32_t CompileShader(const std::string& source, uint32_t type);
private:
	uint32_t m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocations;
};

