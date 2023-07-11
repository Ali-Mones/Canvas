#include "Renderer.h"

static const uint32_t maxQuadCount = 1000;
static const uint32_t maxVertexCount = maxQuadCount * 4;
static const uint32_t maxIndexCount = maxQuadCount * 6;
static uint32_t indexCount = 0;

Renderer::Renderer(const VertexBuffer vertexBuffer, const Shader& shader)
	: m_VertexBuffer(vertexBuffer), m_Shader(shader)
{
}

Renderer::Renderer(const Shader& shader, uint32_t maxQuadCount)
	: m_Shader(shader), m_VertexBuffer(maxQuadCount * 4 * sizeof(Vertex))
{
	std::vector<uint32_t> indices;
	for (int i = 0; i < maxQuadCount; i++)
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

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	//shader.Bind();
	va.Bind();
	ib.Bind();
	
	glDrawElements(GL_TRIANGLES, ib.Count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::BatchStart()
{
	m_Vertices.clear();
	indexCount = 0;
}

void Renderer::BatchEnd()
{
	m_VertexBuffer.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(Vertex), m_Vertices.data());
}

void Renderer::Flush()
{
	//m_Shader.Bind();
	m_VertexArray.Bind();
	m_IndexBuffer.Bind();

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Quad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour)
{
	if (m_Vertices.size() == maxVertexCount)
	{
		BatchEnd();
		Flush();
		BatchStart();
	}

	Vertex v1 =
	{
		{ pos.x, pos.y, pos.z, 1 },
		{ colour.r, colour.g, colour.b, colour.a },
		{ 0, 0 },
		1
	};

	Vertex v2 =
	{
		{ pos.x + dims.x, pos.y, pos.z, 1 },
		{ colour.r, colour.g, colour.b, colour.a },
		{ 1, 0 },
		1
	};

	Vertex v3 =
	{
		{ pos.x + dims.x, pos.y + dims.y, pos.z, 1 },
		{ colour.r, colour.g, colour.b, colour.a },
		{ 1, 1 },
		1
	};

	Vertex v4 =
	{
		{ pos.x, pos.y + dims.y, pos.z, 1 },
		{ colour.r, colour.g, colour.b, colour.a },
		{ 0, 1 },
		1
	};

	indexCount += 6;

	m_Vertices.push_back(v1);
	m_Vertices.push_back(v2);
	m_Vertices.push_back(v3);
	m_Vertices.push_back(v4);
}
