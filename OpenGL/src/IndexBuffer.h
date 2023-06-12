#pragma once
#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t* data, uint32_t count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline uint32_t Count() const { return m_Count; }
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};

