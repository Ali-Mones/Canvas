#pragma once
#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer(const uint32_t maxSize);
	VertexBuffer(const uint32_t size, const void* data);
	~VertexBuffer();
	void Bind();
	void Unbind();
	void SetBuffer(const uint32_t size, const void* data);
private:
	uint32_t m_RendererID;
};

