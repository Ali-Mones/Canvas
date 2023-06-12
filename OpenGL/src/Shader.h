#pragma once
#include <string>
#include <stdint.h>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexShader;
	std::string FragmentShader;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filePath);
	uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	uint32_t CompileShader(const std::string& source, uint32_t type);
private:
	uint32_t m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocations;
};

