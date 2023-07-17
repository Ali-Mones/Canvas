#include "VertexArray.h"
#include "Vertex.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

template<>
void VertexArray::SetLayout<QuadVertex>()
{
	Bind();
	uint32_t offset = offsetof(QuadVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	offset = offsetof(QuadVertex, Colour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	offset = offsetof(QuadVertex, TexCoords);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	offset = offsetof(QuadVertex, TexIndex);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);
}

template<>
void VertexArray::SetLayout<CircleVertex>()
{
	Bind();
	uint32_t offset = offsetof(CircleVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, LocalPosition);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, Colour);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, Thickness);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, Fade);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}
