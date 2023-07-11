#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Vertex.h"

class Renderer
{
public:
	Renderer(const VertexBuffer vertexBuffer, const Shader& shader);
	Renderer(const Shader& shader, uint32_t maxQuadCount);
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void BatchStart();
	void BatchEnd();
	void Flush();
	void Quad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour);

private:
	std::vector<Vertex> m_Vertices;
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;
};

