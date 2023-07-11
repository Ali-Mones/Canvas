#pragma once
#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer(uint32_t maxSize);
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
private:
	uint32_t m_RendererID;
};
