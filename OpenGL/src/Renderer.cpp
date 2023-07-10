#include "Renderer.h"
#include <cstdint>

static const uint32_t maxQuads = 1000;
static const uint32_t maxIndices = maxQuads * 6;
static const uint32_t maxVertices = maxQuads * 4;

static uint32_t indices = 0;

Renderer::Renderer(Shader shader)
	: m_Vertices(std::vector<Vertex>()), m_Shader(shader), m_VertexBuffer(maxVertices)
{
	std::vector<uint32_t> indices;
	for (int i = 0; i < 1000; i++)
	{
		indices.push_back(0 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(3 + i * 4);
		indices.push_back(0 + i * 4);
	}
	m_IndexBuffer = IndexBuffer(indices.data(), indices.size());
	m_Shader.Bind();
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::StartBatch()
{
	m_Vertices.clear();
	indices = 0;
}

void Renderer::EndBatch()
{
	m_VertexBuffer.Bind();
	m_VertexBuffer.SetBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
}


void Renderer::Flush()
{
	m_VertexArray.Bind();
	m_IndexBuffer.Bind();
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	
	glDrawElements(GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Quad(glm::vec2 pos, glm::vec2 dims, glm::vec4 colour)
{
	if (m_Vertices.size() + 4 > maxVertices)
	{
		EndBatch();
		Flush();
		StartBatch();
	}

	Vertex v1 =
	{
		{ pos.x, pos.y, 0, 1 },
		colour,
		{ 0, 0 },
		1
	};
	m_Vertices.push_back(v1);

	Vertex v2 =
	{
		{ pos.x + dims.x, pos.y, 0, 1 },
		colour,
		{ 1, 0 },
		1
	};
	m_Vertices.push_back(v2);

	Vertex v3 =
	{
		{ pos.x + dims.x, pos.y + dims.y, 0, 1 },
		colour,
		{ 1, 1 },
		1
	};
	m_Vertices.push_back(v3);

	Vertex v4 =
	{
		{ pos.x, pos.y + dims.y, 0, 1 },
		colour,
		{ 0, 1 },
		1
	};
	m_Vertices.push_back(v4);

	indices += 6;
}

std::vector<Vertex> Renderer::Rect(glm::vec2 pos, glm::vec2 dims, glm::vec4 colour)
{
	Vertex v1 =
	{
		{ pos.x, pos.y, 0, 1 },
		colour,
		{ 0, 0 },
		1
	};

	Vertex v2 =
	{
		{ pos.x + dims.x, pos.y, 0, 1 },
		colour,
		{ 1, 0 },
		1
	};

	Vertex v3 =
	{
		{ pos.x + dims.x, pos.y + dims.y, 0, 1 },
		colour,
		{ 1, 1 },
		1
	};

	Vertex v4 =
	{
		{ pos.x, pos.y + dims.y, 0, 1 },
		colour,
		{ 0, 1 },
		1
	};

	return { v1, v2, v3, v4 };
}
