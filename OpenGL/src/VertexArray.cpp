#include "VertexArray.h"
#include "Vertex.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);

	uint32_t offset = offsetof(Vertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	offset = offsetof(Vertex, Colour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	offset = offsetof(Vertex, TexCoords);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	offset = offsetof(Vertex, TexIndex);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);
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

