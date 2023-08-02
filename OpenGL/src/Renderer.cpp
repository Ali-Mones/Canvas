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
	QuadVertex* QuadVerticesBase;
	QuadVertex* QuadVerticesCurr;
	Shader* QuadShader;

	uint32_t CircleIndexCount = 0;
	VertexArray* CircleVertexArray;
	VertexBuffer* CircleVertexBuffer;
	CircleVertex* CircleVerticesBase;
	CircleVertex* CircleVerticesCurr;
	Shader* CircleShader;

	uint32_t LineVertexCount = 0;
	VertexArray* LineVertexArray;
	VertexBuffer* LineVertexBuffer;
	LineVertex* LineVerticesBase;
	LineVertex* LineVerticesCurr;
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
	s_RenderData.QuadVerticesBase = new QuadVertex[s_RenderData.MaxVertexCount];
	s_RenderData.QuadVerticesCurr = s_RenderData.QuadVerticesBase;

	// Circle initialisation
	s_RenderData.CircleShader = new Shader("res/shaders/CircleShader.shader");
	s_RenderData.CircleVertexArray = new VertexArray();
	s_RenderData.CircleVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(CircleVertex));
	s_RenderData.CircleVertexArray->SetLayout<CircleVertex>();
	s_RenderData.CircleVerticesBase = new CircleVertex[s_RenderData.MaxVertexCount];
	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;

	// Line initialisation
	s_RenderData.LineShader = new Shader("res/shaders/LineShader.shader");
	s_RenderData.LineVertexArray = new VertexArray();
	s_RenderData.LineVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(LineVertex));
	s_RenderData.LineVertexArray->SetLayout<LineVertex>();
	s_RenderData.LineVerticesBase = new LineVertex[s_RenderData.MaxVertexCount];
	s_RenderData.LineVerticesCurr = s_RenderData.LineVerticesBase;

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
	s_RenderData.QuadVerticesCurr = s_RenderData.QuadVerticesBase;
	s_RenderData.QuadIndexCount = 0;

	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;
	s_RenderData.CircleIndexCount = 0;

	s_RenderData.LineVerticesCurr = s_RenderData.LineVerticesBase;
	s_RenderData.LineVertexCount = 0;
}

void Renderer::Flush()
{
	if (s_RenderData.QuadVerticesBase != s_RenderData.QuadVerticesCurr)
	{
		uint32_t count = s_RenderData.QuadVerticesCurr - s_RenderData.QuadVerticesBase;
		s_RenderData.QuadVertexBuffer->SetBuffer(count * sizeof(QuadVertex), s_RenderData.QuadVerticesBase);
		s_RenderData.QuadVertexArray->Bind();
		s_RenderData.QuadShader->Bind();
		s_RenderData.QuadIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
		s_RenderData.DrawCalls++;
	}

	if (s_RenderData.CircleVerticesBase != s_RenderData.CircleVerticesCurr)
	{
		uint32_t count = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
		s_RenderData.CircleVertexBuffer->SetBuffer(count * sizeof(CircleVertex), s_RenderData.CircleVerticesBase);
		s_RenderData.CircleVertexArray->Bind();
		s_RenderData.CircleShader->Bind();
		s_RenderData.QuadIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.CircleIndexCount, GL_UNSIGNED_INT, nullptr);
		s_RenderData.DrawCalls++;
	}

	if (s_RenderData.LineVerticesBase != s_RenderData.LineVerticesCurr)
	{
		uint32_t count = s_RenderData.LineVerticesCurr - s_RenderData.LineVerticesBase;
		s_RenderData.LineVertexBuffer->SetBuffer(count * sizeof(LineVertex), s_RenderData.LineVerticesBase);
		s_RenderData.LineVertexArray->Bind();
		s_RenderData.LineShader->Bind();
		glDrawArrays(GL_LINES, 0, s_RenderData.LineVertexCount);
		s_RenderData.DrawCalls++;
	}
}

void Renderer::Shutdown()
{
	delete s_RenderData.QuadIndexBuffer;

	delete s_RenderData.QuadVertexArray;
	delete s_RenderData.QuadVertexBuffer;
	delete s_RenderData.QuadShader;
	delete s_RenderData.QuadVerticesBase;

	delete s_RenderData.CircleVertexArray;
	delete s_RenderData.CircleVertexBuffer;
	delete s_RenderData.CircleShader;
	delete s_RenderData.CircleVerticesBase;

	delete s_RenderData.LineVertexArray;
	delete s_RenderData.LineVertexBuffer;
	delete s_RenderData.LineShader;
	delete s_RenderData.LineVerticesBase;
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
	uint32_t vertexCount = s_RenderData.QuadVerticesCurr - s_RenderData.QuadVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 }) * glm::scale(glm::mat4(1), dims);

	s_RenderData.QuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[0];
	s_RenderData.QuadVerticesCurr->Colour = colour;
	s_RenderData.QuadVerticesCurr->TexCoords = { 0, 0 };
	s_RenderData.QuadVerticesCurr->TexIndex = 1;
	s_RenderData.QuadVerticesCurr++;

	s_RenderData.QuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[1];
	s_RenderData.QuadVerticesCurr->Colour = colour;
	s_RenderData.QuadVerticesCurr->TexCoords = { 1, 0 };
	s_RenderData.QuadVerticesCurr->TexIndex = 1;
	s_RenderData.QuadVerticesCurr++;

	s_RenderData.QuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[2];
	s_RenderData.QuadVerticesCurr->Colour = colour;
	s_RenderData.QuadVerticesCurr->TexCoords = { 1, 1 };
	s_RenderData.QuadVerticesCurr->TexIndex = 1;
	s_RenderData.QuadVerticesCurr++;

	s_RenderData.QuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[3];
	s_RenderData.QuadVerticesCurr->Colour = colour;
	s_RenderData.QuadVerticesCurr->TexCoords = { 0, 1 };
	s_RenderData.QuadVerticesCurr->TexIndex = 1;
	s_RenderData.QuadVerticesCurr++;

	s_RenderData.QuadIndexCount += 6;

	s_RenderData.QuadCount++;
}

void Renderer::RenderCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness, float fade)
{
	uint32_t vertexCount = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), dims);

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[0];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[0] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = colour;
	s_RenderData.CircleVerticesCurr->Thickness = thickness;
	s_RenderData.CircleVerticesCurr->Fade = fade;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[1];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[1] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = colour;
	s_RenderData.CircleVerticesCurr->Thickness = thickness;
	s_RenderData.CircleVerticesCurr->Fade = fade;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[2];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[2] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = colour;
	s_RenderData.CircleVerticesCurr->Thickness = thickness;
	s_RenderData.CircleVerticesCurr->Fade = fade;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[3];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[3] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = colour;
	s_RenderData.CircleVerticesCurr->Thickness = thickness;
	s_RenderData.CircleVerticesCurr->Fade = fade;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleIndexCount += 6;

	s_RenderData.QuadCount++;
}

void Renderer::RenderLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight)
{
	uint32_t vertexCount = s_RenderData.LineVerticesCurr - s_RenderData.LineVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	s_RenderData.LineVerticesCurr->Position = glm::vec4(p1, 1.0f);
	s_RenderData.LineVerticesCurr->Colour = colour;
	s_RenderData.LineVerticesCurr++;

	s_RenderData.LineVerticesCurr->Position = glm::vec4(p2, 1.0f);
	s_RenderData.LineVerticesCurr->Colour = colour;
	s_RenderData.LineVerticesCurr++;

	glLineWidth(weight);

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
