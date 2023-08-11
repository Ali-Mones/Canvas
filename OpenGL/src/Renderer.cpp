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

	uint32_t FilledQuadIndexCount = 0;
	VertexArray* FilledQuadVertexArray;
	VertexBuffer* FilledQuadVertexBuffer;
	FilledQuadVertex* FilledQuadVerticesBase;
	FilledQuadVertex* FilledQuadVerticesCurr;
	Shader* FilledQuadShader;

	uint32_t HollowQuadIndexCount = 0;
	VertexArray* HollowQuadVertexArray;
	VertexBuffer* HollowQuadVertexBuffer;
	HollowQuadVertex* HollowQuadVerticesBase;
	HollowQuadVertex* HollowQuadVerticesCurr;
	Shader* HollowQuadShader;

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

	/* Statistics */
	uint32_t QuadCount = 0;
	uint32_t LineCount = 0;
	uint32_t DrawCalls = 0;

	const uint32_t MaxQuadCount = 500;
	const uint32_t MaxVertexCount = MaxQuadCount * 4;
	const uint32_t MaxIndexCount = MaxQuadCount * 6;
};

static RenderData s_RenderData;

void Renderer::Init()
{
	// Filled Quad initialisation
	s_RenderData.FilledQuadShader = new Shader("res/shaders/Quad.shader");
	s_RenderData.FilledQuadVertexArray = new VertexArray();
	s_RenderData.FilledQuadVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(FilledQuadVertex));
	s_RenderData.FilledQuadVertexArray->SetLayout<FilledQuadVertex>();
	s_RenderData.FilledQuadVerticesBase = new FilledQuadVertex[s_RenderData.MaxVertexCount];
	s_RenderData.FilledQuadVerticesCurr = s_RenderData.FilledQuadVerticesBase;

	// Hollow Quad initialisation
	s_RenderData.HollowQuadShader = new Shader("res/shaders/HollowQuad.shader");
	s_RenderData.HollowQuadVertexArray = new VertexArray();
	s_RenderData.HollowQuadVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(HollowQuadVertex));
	s_RenderData.HollowQuadVertexArray->SetLayout<HollowQuadVertex>();
	s_RenderData.HollowQuadVerticesBase = new HollowQuadVertex[s_RenderData.MaxVertexCount];
	s_RenderData.HollowQuadVerticesCurr = s_RenderData.HollowQuadVerticesBase;

	// Circle initialisation
	s_RenderData.CircleShader = new Shader("res/shaders/Circle.shader");
	s_RenderData.CircleVertexArray = new VertexArray();
	s_RenderData.CircleVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(CircleVertex));
	s_RenderData.CircleVertexArray->SetLayout<CircleVertex>();
	s_RenderData.CircleVerticesBase = new CircleVertex[s_RenderData.MaxVertexCount];
	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;

	// Line initialisation
	s_RenderData.LineShader = new Shader("res/shaders/Line.shader");
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

	s_RenderData.UnitQuadVertices[0] = glm::vec4(-0.5, -0.5, 0, 1);
	s_RenderData.UnitQuadVertices[1] = glm::vec4( 0.5, -0.5, 0, 1);
	s_RenderData.UnitQuadVertices[2] = glm::vec4( 0.5,  0.5, 0, 1);
	s_RenderData.UnitQuadVertices[3] = glm::vec4(-0.5,  0.5, 0, 1);
}

void Renderer::StartBatch()
{
	s_RenderData.FilledQuadVerticesCurr = s_RenderData.FilledQuadVerticesBase;
	s_RenderData.FilledQuadIndexCount = 0;

	s_RenderData.HollowQuadVerticesCurr = s_RenderData.HollowQuadVerticesBase;
	s_RenderData.HollowQuadIndexCount = 0;

	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;
	s_RenderData.CircleIndexCount = 0;

	s_RenderData.LineVerticesCurr = s_RenderData.LineVerticesBase;
	s_RenderData.LineVertexCount = 0;
}

void Renderer::Flush()
{
	if (s_RenderData.FilledQuadVerticesBase != s_RenderData.FilledQuadVerticesCurr)
	{
		uint32_t count = s_RenderData.FilledQuadVerticesCurr - s_RenderData.FilledQuadVerticesBase;
		s_RenderData.FilledQuadVertexBuffer->SetBuffer(count * sizeof(FilledQuadVertex), s_RenderData.FilledQuadVerticesBase);
		s_RenderData.FilledQuadVertexArray->Bind();
		s_RenderData.FilledQuadShader->Bind();
		s_RenderData.QuadIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.FilledQuadIndexCount, GL_UNSIGNED_INT, nullptr);
		s_RenderData.DrawCalls++;
	}

	if (s_RenderData.HollowQuadVerticesBase != s_RenderData.HollowQuadVerticesCurr)
	{
		uint32_t count = s_RenderData.HollowQuadVerticesCurr - s_RenderData.HollowQuadVerticesBase;
		s_RenderData.HollowQuadVertexBuffer->SetBuffer(count * sizeof(HollowQuadVertex), s_RenderData.HollowQuadVerticesBase);
		s_RenderData.HollowQuadVertexArray->Bind();
		s_RenderData.HollowQuadShader->Bind();
		s_RenderData.QuadIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.HollowQuadIndexCount, GL_UNSIGNED_INT, nullptr);
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

	delete s_RenderData.FilledQuadVertexArray;
	delete s_RenderData.FilledQuadVertexBuffer;
	delete s_RenderData.FilledQuadShader;
	delete s_RenderData.FilledQuadVerticesBase;

	delete s_RenderData.HollowQuadVertexArray;
	delete s_RenderData.HollowQuadVertexBuffer;
	delete s_RenderData.HollowQuadShader;
	delete s_RenderData.HollowQuadVerticesBase;

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderFilledQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation, float stroke)
{
	uint32_t vertexCount = s_RenderData.FilledQuadVerticesCurr - s_RenderData.FilledQuadVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	if (stroke != 0.0f)
		RenderFilledQuad(glm::vec3(pos.x, pos.y, 0), glm::vec3(dims.x + stroke, dims.y + stroke, 0), glm::vec4(1), rotation);

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1), rotation, { 0, 0, 1 }) * glm::scale(glm::mat4(1), dims);

	s_RenderData.FilledQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[0];
	s_RenderData.FilledQuadVerticesCurr->Colour = colour;
	s_RenderData.FilledQuadVerticesCurr->TexCoords = glm::vec2(0, 0);
	s_RenderData.FilledQuadVerticesCurr->TexIndex = 1;
	s_RenderData.FilledQuadVerticesCurr++;

	s_RenderData.FilledQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[1];
	s_RenderData.FilledQuadVerticesCurr->Colour = colour;
	s_RenderData.FilledQuadVerticesCurr->TexCoords = glm::vec2(1, 0);
	s_RenderData.FilledQuadVerticesCurr->TexIndex = 1;
	s_RenderData.FilledQuadVerticesCurr++;

	s_RenderData.FilledQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[2];
	s_RenderData.FilledQuadVerticesCurr->Colour = colour;
	s_RenderData.FilledQuadVerticesCurr->TexCoords = glm::vec2(1, 1);
	s_RenderData.FilledQuadVerticesCurr->TexIndex = 1;
	s_RenderData.FilledQuadVerticesCurr++;

	s_RenderData.FilledQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[3];
	s_RenderData.FilledQuadVerticesCurr->Colour = colour;
	s_RenderData.FilledQuadVerticesCurr->TexCoords = glm::vec2(0, 1);
	s_RenderData.FilledQuadVerticesCurr->TexIndex = 1;
	s_RenderData.FilledQuadVerticesCurr++;

	s_RenderData.FilledQuadIndexCount += 6;

	s_RenderData.QuadCount++;
}

void Renderer::RenderHollowQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness)
{
	uint32_t vertexCount = s_RenderData.HollowQuadVerticesCurr - s_RenderData.HollowQuadVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), pos) * glm::rotate(glm::mat4(1), 0.0f, { 0, 0, 1 }) * glm::scale(glm::mat4(1), dims);

	s_RenderData.HollowQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[0];
	s_RenderData.HollowQuadVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[0] * 2.0f;
	s_RenderData.HollowQuadVerticesCurr->Colour = colour;
	s_RenderData.HollowQuadVerticesCurr->Thickness = thickness;
	s_RenderData.HollowQuadVerticesCurr++;

	s_RenderData.HollowQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[1];
	s_RenderData.HollowQuadVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[1] * 2.0f;
	s_RenderData.HollowQuadVerticesCurr->Colour = colour;
	s_RenderData.HollowQuadVerticesCurr->Thickness = thickness;
	s_RenderData.HollowQuadVerticesCurr++;

	s_RenderData.HollowQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[2];
	s_RenderData.HollowQuadVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[2] * 2.0f;
	s_RenderData.HollowQuadVerticesCurr->Colour = colour;
	s_RenderData.HollowQuadVerticesCurr->Thickness = thickness;
	s_RenderData.HollowQuadVerticesCurr++;

	s_RenderData.HollowQuadVerticesCurr->Position = transform * s_RenderData.UnitQuadVertices[3];
	s_RenderData.HollowQuadVerticesCurr->LocalPosition = s_RenderData.UnitQuadVertices[3] * 2.0f;
	s_RenderData.HollowQuadVerticesCurr->Colour = colour;
	s_RenderData.HollowQuadVerticesCurr->Thickness = thickness;
	s_RenderData.HollowQuadVerticesCurr++;

	s_RenderData.HollowQuadIndexCount += 6;

	s_RenderData.QuadCount++;
}

void Renderer::RenderCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness, float fade, float stroke)
{
	uint32_t vertexCount = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	if (stroke != 0.0f)
		RenderCircle(pos, glm::vec3(dims.x + stroke, dims.y + stroke, 0), glm::vec4(1), 1, fade);

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

Shader* Renderer::FilledQuadShader()
{
	return s_RenderData.FilledQuadShader;
}

Shader* Renderer::HollowQuadShader()
{
	return s_RenderData.HollowQuadShader;
}

Shader* Renderer::CircleShader()
{
	return s_RenderData.CircleShader;
}

Shader* Renderer::LineShader()
{
	return s_RenderData.LineShader;
}
