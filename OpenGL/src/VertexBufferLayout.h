#pragma once
#include <vector>
#include <glew.h>

struct VertexLayoutElement
{
	uint32_t type;
	uint32_t count;
	bool normalised;

	static uint32_t GetSizeOfType(uint32_t type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}

		return 0;
	}
};

class VertexBufferLayout
{
public:
	template<typename T>
	void Push(uint32_t count);

	template<>
	void Push<float>(uint32_t count)
	{
		m_Elements.push_back({ GL_FLOAT, count, false });
		m_Stride += count * VertexLayoutElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<uint32_t>(uint32_t count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_Stride += count * VertexLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<uint8_t>(uint32_t count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, true });
		m_Stride += count * VertexLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	const uint32_t Stride() const { return m_Stride; }
	const std::vector<VertexLayoutElement>& Elements() const { return m_Elements; }

private:
	std::vector<VertexLayoutElement> m_Elements;
	uint32_t m_Stride = 0;
};