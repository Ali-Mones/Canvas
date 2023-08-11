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
void VertexArray::SetLayout<FilledQuadVertex>()
{
	Bind();
	uint32_t offset = offsetof(FilledQuadVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FilledQuadVertex), (const void*) offset);

	offset = offsetof(FilledQuadVertex, Colour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(FilledQuadVertex), (const void*) offset);

	offset = offsetof(FilledQuadVertex, TexCoords);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FilledQuadVertex), (const void*) offset);

	offset = offsetof(FilledQuadVertex, TexIndex);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(FilledQuadVertex), (const void*) offset);
}

template<>
void VertexArray::SetLayout<HollowQuadVertex>()
{
	Bind();
	uint32_t offset = offsetof(HollowQuadVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(HollowQuadVertex), (const void*) offset);

	offset = offsetof(HollowQuadVertex, LocalPosition);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(HollowQuadVertex), (const void*) offset);

	offset = offsetof(HollowQuadVertex, Colour);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(HollowQuadVertex), (const void*) offset);

	offset = offsetof(HollowQuadVertex, Thickness);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(HollowQuadVertex), (const void*) offset);
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

template<>
void VertexArray::SetLayout<LineVertex>()
{
	Bind();
	uint32_t offset = offsetof(LineVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*) offset);

	offset = offsetof(LineVertex, Colour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(LineVertex), (const void*) offset);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}
