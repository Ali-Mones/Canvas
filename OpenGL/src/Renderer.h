#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Vertex.h"

class Renderer
{
public:
	Renderer(Shader shader);
	void Clear();
	void StartBatch();
	void EndBatch();
	void Flush();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	void Quad(glm::vec2 pos, glm::vec2 dims, glm::vec4 colour);
	std::vector<Vertex> Rect(glm::vec2 pos, glm::vec2 dims, glm::vec4 colour);

private:
	std::vector<Vertex> m_Vertices;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	VertexArray m_VertexArray;
	Shader m_Shader;
};
