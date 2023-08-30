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
void VertexArray::SetLayout<RectVertex>()
{
	Bind();
	uint32_t offset = offsetof(RectVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, LocalPosition);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, FillColour);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, StrokeColour);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, ThicknessX);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, ThicknessY);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, TexCoords);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, TexIndex);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*) offset);

	offset = offsetof(RectVertex, TilingFactor);
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (const void*)offset);
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

	offset = offsetof(CircleVertex, FillColour);
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
