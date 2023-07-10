#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddBuffer(const VertexBuffer& vb) const;
private:
	uint32_t m_RendererID;
};

