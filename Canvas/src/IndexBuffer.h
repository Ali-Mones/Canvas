#pragma once

#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t count, const void* data);
	~IndexBuffer();
	void Bind();
	void Unbind();
private:
	uint32_t m_RendererID;
};
