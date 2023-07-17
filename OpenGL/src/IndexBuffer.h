#pragma once

#include <glew.h>
#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t count, const void* data);
	~IndexBuffer();
	void Bind();
	void Unbind();
	void SetBuffer(const uint32_t size, const void* data);
	uint32_t Count() { return m_Count; }
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};
