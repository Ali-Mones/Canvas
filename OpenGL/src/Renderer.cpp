#include "Renderer.h"
#include <glew.h>
#include <glm/ext/matrix_transform.hpp>

static const uint32_t s_MaxQuadCount = 1000;
static const uint32_t s_MaxVertexCount = s_MaxQuadCount * 4;
static const uint32_t s_MaxIndexCount = s_MaxQuadCount * 6;
static uint32_t s_IndexCount = 0;
static GLuint s_VertexArray;
static GLuint s_VertexBuffer;
static GLuint s_IndexBuffer;
static std::vector<Vertex> s_Vertices;
static std::vector<glm::vec4> s_QuadVertices;

//Renderer::Renderer(const VertexBuffer vertexBuffer, const Shader& shader)
//	: m_VertexBuffer(vertexBuffer), m_Shader(shader)
//{
//}

//Renderer::Renderer(const Shader& shader, uint32_t maxQuadCount)
//	: m_Shader(shader), m_VertexBuffer(maxQuadCount * 4 * sizeof(Vertex))
//{
//	std::vector<uint32_t> indices;
//	for (int i = 0; i < maxQuadCount; i++)
//	{
//		indices.push_back(0 + i * 4);
//		indices.push_back(1 + i * 4);
//		indices.push_back(2 + i * 4);
//		indices.push_back(2 + i * 4);
//		indices.push_back(3 + i * 4);
//		indices.push_back(0 + i * 4);
//	}
//
//	m_IndexBuffer = IndexBuffer(indices.data(), indices.size());
//	m_Shader.Bind();
//}

void Renderer::Init()
{
	glGenVertexArrays(1, &s_VertexArray);
	glBindVertexArray(s_VertexArray);

	glGenBuffers(1, &s_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, s_MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	uint32_t offset = offsetof(Vertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	offset = offsetof(Vertex, Colour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	offset = offsetof(Vertex, TexCoords);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	offset = offsetof(Vertex, TexIndex);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) offset);

	glGenBuffers(1, &s_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IndexBuffer);

	std::vector<uint32_t> indices;
	for (int i = 0; i < s_MaxQuadCount; i++)
	{
		indices.push_back(0 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(3 + i * 4);
		indices.push_back(0 + i * 4);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	s_QuadVertices.push_back({ -0.5, -0.5, 0, 1 });
	s_QuadVertices.push_back({  0.5, -0.5, 0, 1 });
	s_QuadVertices.push_back({  0.5,  0.5, 0, 1 });
	s_QuadVertices.push_back({ -0.5,  0.5, 0, 1 });
}

void Renderer::BatchStart()
{
	s_Vertices.clear();
	s_IndexCount = 0;
}

void Renderer::BatchEnd()
{
	glBindBuffer(GL_ARRAY_BUFFER, s_VertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, s_Vertices.size() * sizeof(Vertex), s_Vertices.data());
}

void Renderer::Flush()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IndexBuffer);
	glBindVertexArray(s_VertexArray);

	glDrawElements(GL_TRIANGLES, s_IndexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Shutdown()
{
	glDeleteBuffers(1, &s_IndexBuffer);
	glDeleteBuffers(1, &s_VertexBuffer);
	glDeleteVertexArrays(1, &s_VertexArray);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Quad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation)
{
	if (s_Vertices.size() == s_MaxVertexCount)
	{
		BatchEnd();
		Flush();
		BatchStart();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 }) * glm::scale(glm::mat4(1), dims);

	Vertex v1 =
	{
		transform * s_QuadVertices[0],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 0, 0 },
		1
	};

	Vertex v2 =
	{
		transform* s_QuadVertices[1],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 1, 0 },
		1
	};

	Vertex v3 =
	{
		transform* s_QuadVertices[2],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 1, 1 },
		1
	};

	Vertex v4 =
	{
		transform* s_QuadVertices[3],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 0, 1 },
		1
	};

	s_IndexCount += 6;

	s_Vertices.push_back(v1);
	s_Vertices.push_back(v2);
	s_Vertices.push_back(v3);
	s_Vertices.push_back(v4);
}
