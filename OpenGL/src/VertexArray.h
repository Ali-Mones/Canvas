#pragma once

#include <glew.h>
#include <cstdint>
#include <string>
#include <vector>

struct VertexBufferLayoutElement
{
	std::string Name;
	uint32_t Type;
	uint32_t Count;
	bool Normalised = false;

	uint32_t SizeOfType()
	{
		switch (Type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}

		return 0;
	}
};
class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	template <typename T> 
	void SetLayout();

	void Bind();
	void Unbind();
private:
	uint32_t m_RendererID;
};
