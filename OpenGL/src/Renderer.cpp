#include "Renderer.h"
#include "Shader.h"
#include <glew.h>
#include <glm/ext/matrix_transform.hpp>

struct RenderData
{
	uint32_t QuadIndexBuffer;

	uint32_t QuadIndexCount = 0;
	uint32_t QuadVertexArray;
	uint32_t QuadVertexBuffer;
	std::vector<QuadVertex> QuadVertices;
	Shader* QuadShader;

	uint32_t CircleIndexCount = 0;
	uint32_t CircleVertexArray;
	uint32_t CircleVertexBuffer;
	std::vector<CircleVertex> CircleVertices;
	Shader* CircleShader;

	std::vector<glm::vec4> UnitQuadVertices;

	// stats
	const uint32_t MaxQuadCount = 1000;
	const uint32_t MaxVertexCount = MaxQuadCount * 4;
	const uint32_t MaxIndexCount = MaxQuadCount * 6;

	uint32_t QuadCount = 0;
	uint32_t DrawCalls = 0;
};

static RenderData s_RenderData;

void Renderer::Init()
{
	// Quad initialisation
	s_RenderData.QuadShader = new Shader("res/shaders/QuadShader.shader");

	glGenVertexArrays(1, &s_RenderData.QuadVertexArray);
	glBindVertexArray(s_RenderData.QuadVertexArray);

	glGenBuffers(1, &s_RenderData.QuadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_RenderData.QuadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, s_RenderData.MaxVertexCount * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

	uint32_t offset = offsetof(QuadVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	offset = offsetof(QuadVertex, Colour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	offset = offsetof(QuadVertex, TexCoords);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	offset = offsetof(QuadVertex, TexIndex);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*) offset);

	// Circle initialisation
	s_RenderData.CircleShader = new Shader("res/shaders/CircleShader.shader");

	glGenVertexArrays(1, &s_RenderData.CircleVertexArray);
	glBindVertexArray(s_RenderData.CircleVertexArray);

	glGenBuffers(1, &s_RenderData.CircleVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, s_RenderData.CircleVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, s_RenderData.MaxVertexCount * sizeof(CircleVertex), nullptr, GL_DYNAMIC_DRAW);

	offset = offsetof(CircleVertex, Position);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, LocalPosition);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, Colour);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, Thickness);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	offset = offsetof(CircleVertex, Fade);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(CircleVertex), (const void*) offset);

	// Index buffer initialisation
	glGenBuffers(1, &s_RenderData.QuadIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RenderData.QuadIndexBuffer);

	std::vector<uint32_t> indices;
	for (int i = 0; i < s_RenderData.MaxQuadCount; i++)
	{
		indices.push_back(0 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(3 + i * 4);
		indices.push_back(0 + i * 4);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	s_RenderData.UnitQuadVertices.push_back({ -0.5, -0.5, 0, 1 });
	s_RenderData.UnitQuadVertices.push_back({  0.5, -0.5, 0, 1 });
	s_RenderData.UnitQuadVertices.push_back({  0.5,  0.5, 0, 1 });
	s_RenderData.UnitQuadVertices.push_back({ -0.5,  0.5, 0, 1 });
}

void Renderer::BatchStart()
{
	s_RenderData.QuadVertices.clear();
	s_RenderData.QuadIndexCount = 0;

	s_RenderData.CircleVertices.clear();
	s_RenderData.CircleIndexCount = 0;
}

void Renderer::BatchEnd()
{
	glBindBuffer(GL_ARRAY_BUFFER, s_RenderData.QuadVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, s_RenderData.QuadVertices.size() * sizeof(QuadVertex), s_RenderData.QuadVertices.data());

	glBindBuffer(GL_ARRAY_BUFFER, s_RenderData.CircleVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, s_RenderData.CircleVertices.size() * sizeof(CircleVertex), s_RenderData.CircleVertices.data());
}

void Renderer::Flush()
{
	glBindVertexArray(s_RenderData.QuadVertexArray);
	s_RenderData.QuadShader->Bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RenderData.QuadIndexBuffer);
	glDrawElements(GL_TRIANGLES, s_RenderData.QuadIndexCount, GL_UNSIGNED_INT, nullptr);

	
	glBindVertexArray(s_RenderData.CircleVertexArray);
	s_RenderData.CircleShader->Bind();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RenderData.QuadIndexBuffer);
	glDrawElements(GL_TRIANGLES, s_RenderData.CircleIndexCount, GL_UNSIGNED_INT, nullptr);

	s_RenderData.DrawCalls += 2;
}

void Renderer::Shutdown()
{
	delete s_RenderData.QuadShader;
	delete s_RenderData.CircleShader;

	glDeleteBuffers(1, &s_RenderData.QuadIndexBuffer);

	glDeleteBuffers(1, &s_RenderData.QuadVertexBuffer);
	glDeleteVertexArrays(1, &s_RenderData.QuadVertexArray);

	glDeleteBuffers(1, &s_RenderData.CircleVertexBuffer);
	glDeleteVertexArrays(1, &s_RenderData.CircleVertexArray);
}

void Renderer::Clear(glm::vec4 colour)
{
	s_RenderData.DrawCalls = 0;
	s_RenderData.QuadCount = 0;

	glClearColor(colour.r, colour.g, colour.b, colour.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation)
{
	if (s_RenderData.QuadVertices.size() == s_RenderData.MaxVertexCount)
	{
		BatchEnd();
		Flush();
		BatchStart();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 }) * glm::scale(glm::mat4(1), dims);

	QuadVertex v1 =
	{
		transform * s_RenderData.UnitQuadVertices[0],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 0, 0 },
		1
	};

	QuadVertex v2 =
	{
		transform * s_RenderData.UnitQuadVertices[1],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 1, 0 },
		1
	};

	QuadVertex v3 =
	{
		transform * s_RenderData.UnitQuadVertices[2],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 1, 1 },
		1
	};

	QuadVertex v4 =
	{
		transform * s_RenderData.UnitQuadVertices[3],
		{ colour.r, colour.g, colour.b, colour.a },
		{ 0, 1 },
		1
	};

	s_RenderData.QuadIndexCount += 6;
	
	s_RenderData.QuadVertices.push_back(v1);
	s_RenderData.QuadVertices.push_back(v2);
	s_RenderData.QuadVertices.push_back(v3);
	s_RenderData.QuadVertices.push_back(v4);

	s_RenderData.QuadCount++;
}

void Renderer::RenderCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness, float fade)
{
	if (s_RenderData.CircleVertices.size() == s_RenderData.MaxVertexCount)
	{
		BatchEnd();
		Flush();
		BatchStart();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), dims);

	CircleVertex v1 =
	{
		transform * s_RenderData.UnitQuadVertices[0],
		s_RenderData.UnitQuadVertices[0] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a }
	};

	CircleVertex v2 =
	{
		transform * s_RenderData.UnitQuadVertices[1],
		s_RenderData.UnitQuadVertices[1] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
	};

	CircleVertex v3 =
	{
		transform * s_RenderData.UnitQuadVertices[2],
		s_RenderData.UnitQuadVertices[2] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
	};

	CircleVertex v4 =
	{
		transform * s_RenderData.UnitQuadVertices[3],
		s_RenderData.UnitQuadVertices[3] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
	};

	s_RenderData.CircleIndexCount += 6;

	s_RenderData.CircleVertices.push_back(v1);
	s_RenderData.CircleVertices.push_back(v2);
	s_RenderData.CircleVertices.push_back(v3);
	s_RenderData.CircleVertices.push_back(v4);

	s_RenderData.QuadCount++;
}

uint32_t Renderer::QuadCount()
{
	return s_RenderData.QuadCount;
}

uint32_t Renderer::DrawCalls()
{
	return s_RenderData.DrawCalls;
}

Shader* Renderer::QuadShader()
{
	return s_RenderData.QuadShader;
}

Shader* Renderer::CircleShader()
{
	return s_RenderData.CircleShader;
}
