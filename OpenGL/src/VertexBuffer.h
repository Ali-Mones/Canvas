#pragma once
#include <cstdint>
#include "glew.h"

class VertexBuffer
{
public:
	VertexBuffer(uint32_t maxCount);
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();
	void SetBuffer(const void* data, uint32_t size);
	void Bind() const;
	void Unbind() const;
private:
	uint32_t m_RendererID;
};

