#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(uint32_t maxSize)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(const uint32_t size, const void* data)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetBuffer(const uint32_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
