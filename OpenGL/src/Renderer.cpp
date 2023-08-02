#include "Renderer.h"
#include "Shader.h"
#include <glew.h>
#include <glm/ext/matrix_transform.hpp>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct RenderData
{
	IndexBuffer* QuadIndexBuffer;

	uint32_t QuadIndexCount = 0;
	VertexArray* QuadVertexArray;
	VertexBuffer* QuadVertexBuffer;
	std::vector<QuadVertex> QuadVertices;
	Shader* QuadShader;

	uint32_t CircleIndexCount = 0;
	VertexArray* CircleVertexArray;
	VertexBuffer* CircleVertexBuffer;
	std::vector<CircleVertex> CircleVertices;
	Shader* CircleShader;

	uint32_t LineVertexCount = 0;
	VertexArray* LineVertexArray;
	VertexBuffer* LineVertexBuffer;
	std::vector<LineVertex> LineVertices;
	Shader* LineShader;

	glm::vec4 UnitQuadVertices[4];

	// stats
	uint32_t QuadCount = 0;
	uint32_t LineCount = 0;
	uint32_t DrawCalls = 0;

	const uint32_t MaxQuadCount = 5000;
	const uint32_t MaxVertexCount = MaxQuadCount * 4;
	const uint32_t MaxIndexCount = MaxQuadCount * 6;
};

static RenderData s_RenderData;

void Renderer::Init()
{
	// Quad initialisation
	s_RenderData.QuadShader = new Shader("res/shaders/QuadShader.shader");
	s_RenderData.QuadVertexArray = new VertexArray();
	s_RenderData.QuadVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(QuadVertex));
	s_RenderData.QuadVertexArray->SetLayout<QuadVertex>();

	// Circle initialisation
	s_RenderData.CircleShader = new Shader("res/shaders/CircleShader.shader");
	s_RenderData.CircleVertexArray = new VertexArray();
	s_RenderData.CircleVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(CircleVertex));
	s_RenderData.CircleVertexArray->SetLayout<CircleVertex>();

	// Line initialisation
	s_RenderData.LineShader = new Shader("res/shaders/LineShader.shader");
	s_RenderData.LineVertexArray = new VertexArray();
	s_RenderData.LineVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(LineVertex));
	s_RenderData.LineVertexArray->SetLayout<LineVertex>();

	// Index buffer initialisation
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

	s_RenderData.QuadIndexBuffer = new IndexBuffer(indices.size(), indices.data());

	s_RenderData.UnitQuadVertices[0] = { -0.5, -0.5, 0, 1 };
	s_RenderData.UnitQuadVertices[1] = {  0.5, -0.5, 0, 1 };
	s_RenderData.UnitQuadVertices[2] = {  0.5,  0.5, 0, 1 };
	s_RenderData.UnitQuadVertices[3] = { -0.5,  0.5, 0, 1 };
}

void Renderer::StartBatch()
{
	s_RenderData.QuadVertices.clear();
	s_RenderData.QuadIndexCount = 0;

	s_RenderData.CircleVertices.clear();
	s_RenderData.CircleIndexCount = 0;

	s_RenderData.LineVertices.clear();
	s_RenderData.LineVertexCount = 0;
}

void Renderer::Flush()
{
	s_RenderData.QuadVertexBuffer->SetBuffer(s_RenderData.QuadVertices.size() * sizeof(QuadVertex), s_RenderData.QuadVertices.data());
	s_RenderData.CircleVertexBuffer->SetBuffer(s_RenderData.CircleVertices.size() * sizeof(CircleVertex), s_RenderData.CircleVertices.data());
	s_RenderData.LineVertexBuffer->SetBuffer(s_RenderData.LineVertices.size() * sizeof(LineVertex), s_RenderData.LineVertices.data());

	s_RenderData.QuadVertexArray->Bind();
	s_RenderData.QuadShader->Bind();
	s_RenderData.QuadIndexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, s_RenderData.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
	
	s_RenderData.CircleVertexArray->Bind();
	s_RenderData.CircleShader->Bind();
	s_RenderData.QuadIndexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, s_RenderData.CircleIndexCount, GL_UNSIGNED_INT, nullptr);

	s_RenderData.LineVertexArray->Bind();
	s_RenderData.LineShader->Bind();
	glDrawArrays(GL_LINES, 0, s_RenderData.LineVertexCount);

	s_RenderData.DrawCalls += 3;
}

void Renderer::Shutdown()
{
	delete s_RenderData.QuadIndexBuffer;

	delete s_RenderData.QuadVertexArray;
	delete s_RenderData.QuadVertexBuffer;
	delete s_RenderData.QuadShader;

	delete s_RenderData.CircleVertexArray;
	delete s_RenderData.CircleVertexBuffer;
	delete s_RenderData.CircleShader;
}

void Renderer::Clear(glm::vec4 colour)
{
	s_RenderData.DrawCalls = 0;
	s_RenderData.QuadCount = 0;
	s_RenderData.LineCount = 0;

	glClearColor(colour.r, colour.g, colour.b, colour.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation)
{
	if (s_RenderData.QuadVertices.size() == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
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
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), dims);

	CircleVertex v1 =
	{
		transform * s_RenderData.UnitQuadVertices[0],
		s_RenderData.UnitQuadVertices[0] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
		thickness,
		fade
	};

	CircleVertex v2 =
	{
		transform * s_RenderData.UnitQuadVertices[1],
		s_RenderData.UnitQuadVertices[1] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
		thickness,
		fade
	};

	CircleVertex v3 =
	{
		transform * s_RenderData.UnitQuadVertices[2],
		s_RenderData.UnitQuadVertices[2] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
		thickness,
		fade
	};

	CircleVertex v4 =
	{
		transform * s_RenderData.UnitQuadVertices[3],
		s_RenderData.UnitQuadVertices[3] * 2.0f,
		{ colour.r, colour.g, colour.b, colour.a },
		thickness,
		fade
	};

	s_RenderData.CircleIndexCount += 6;

	s_RenderData.CircleVertices.push_back(v1);
	s_RenderData.CircleVertices.push_back(v2);
	s_RenderData.CircleVertices.push_back(v3);
	s_RenderData.CircleVertices.push_back(v4);

	s_RenderData.QuadCount++;
}

void Renderer::RenderLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight)
{
	if (s_RenderData.LineVertices.size() == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glLineWidth(weight);

	LineVertex v1 =
	{
		glm::vec4(p1, 1.0f),
		colour
	};

	LineVertex v2 =
	{
		glm::vec4(p2, 1.0f),
		colour
	};

	s_RenderData.LineVertices.push_back(v1);
	s_RenderData.LineVertices.push_back(v2);

	s_RenderData.LineCount++;
	s_RenderData.LineVertexCount += 2;
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

Shader* Renderer::LineShader()
{
	return s_RenderData.LineShader;
}
