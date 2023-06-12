#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const 
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.Elements();

	uint32_t offset = 0;

	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		uint32_t size = element.count * VertexLayoutElement::GetSizeOfType(element.type);

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalised ? GL_TRUE : GL_FALSE, layout.Stride(), (const void*) offset);

		offset += size;
	}
}

