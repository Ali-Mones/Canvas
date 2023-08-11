#include "Renderer.h"
#include "Shader.h"
#include <glew.h>
#include <glm/ext/matrix_transform.hpp>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

struct RenderData
{
	IndexBuffer* RectIndexBuffer;

	uint32_t RectIndexCount = 0;
	VertexArray* RectVertexArray;
	VertexBuffer* RectVertexBuffer;
	RectVertex* RectVerticesBase;
	RectVertex* RectVerticesCurr;
	Shader* RectShader;

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

	glm::vec4 UnitRectVertices[4];

	/* Attributes */
	glm::vec4 FillColour = glm::vec4(1);
	glm::vec4 StrokeColour = glm::vec4(1);
	uint32_t StrokeWeight = 0;

	/* Statistics */
	uint32_t RectCount = 0;
	uint32_t LineCount = 0;
	uint32_t DrawCalls = 0;

	const uint32_t MaxRectCount = 500;
	const uint32_t MaxVertexCount = MaxRectCount * 4;
	const uint32_t MaxIndexCount = MaxRectCount * 6;
};

static RenderData s_RenderData;

void Renderer::Init()
{
	//  Rect initialisation
	s_RenderData.RectShader = new Shader("res/shaders/Quad.shader");
	s_RenderData.RectVertexArray = new VertexArray();
	s_RenderData.RectVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(RectVertex));
	s_RenderData.RectVertexArray->SetLayout<RectVertex>();
	s_RenderData.RectVerticesBase = new RectVertex[s_RenderData.MaxVertexCount];
	s_RenderData.RectVerticesCurr = s_RenderData.RectVerticesBase;

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
	for (int i = 0; i < s_RenderData.MaxRectCount; i++)
	{
		indices.push_back(0 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(3 + i * 4);
		indices.push_back(0 + i * 4);
	}

	s_RenderData.RectIndexBuffer = new IndexBuffer(indices.size(), indices.data());

	s_RenderData.UnitRectVertices[0] = glm::vec4(-0.5, -0.5, 0, 1);
	s_RenderData.UnitRectVertices[1] = glm::vec4( 0.5, -0.5, 0, 1);
	s_RenderData.UnitRectVertices[2] = glm::vec4( 0.5,  0.5, 0, 1);
	s_RenderData.UnitRectVertices[3] = glm::vec4(-0.5,  0.5, 0, 1);
}

void Renderer::StartBatch()
{
	s_RenderData.RectVerticesCurr = s_RenderData.RectVerticesBase;
	s_RenderData.RectIndexCount = 0;

	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;
	s_RenderData.CircleIndexCount = 0;

	s_RenderData.LineVerticesCurr = s_RenderData.LineVerticesBase;
	s_RenderData.LineVertexCount = 0;
}

void Renderer::Flush()
{
	if (s_RenderData.RectVerticesBase != s_RenderData.RectVerticesCurr)
	{
		uint32_t count = s_RenderData.RectVerticesCurr - s_RenderData.RectVerticesBase;
		s_RenderData.RectVertexBuffer->SetBuffer(count * sizeof(RectVertex), s_RenderData.RectVerticesBase);
		s_RenderData.RectVertexArray->Bind();
		s_RenderData.RectShader->Bind();
		s_RenderData.RectIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.RectIndexCount, GL_UNSIGNED_INT, nullptr);
		s_RenderData.DrawCalls++;
	}

	if (s_RenderData.CircleVerticesBase != s_RenderData.CircleVerticesCurr)
	{
		uint32_t count = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
		s_RenderData.CircleVertexBuffer->SetBuffer(count * sizeof(CircleVertex), s_RenderData.CircleVerticesBase);
		s_RenderData.CircleVertexArray->Bind();
		s_RenderData.CircleShader->Bind();
		s_RenderData.RectIndexBuffer->Bind();
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
	delete s_RenderData.RectIndexBuffer;

	delete s_RenderData.RectVertexArray;
	delete s_RenderData.RectVertexBuffer;
	delete s_RenderData.RectShader;
	delete s_RenderData.RectVerticesBase;

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
	s_RenderData.RectCount = 0;
	s_RenderData.LineCount = 0;

	glClearColor(colour.r, colour.g, colour.b, colour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Rect(int x, int y, int w, int h)
{
	uint32_t vertexCount = s_RenderData.RectVerticesCurr - s_RenderData.RectVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	if (h == -1)
		h = w;

	if (s_RenderData.StrokeWeight != 0)
	{
		glm::vec4 fillColour(s_RenderData.FillColour);
		uint32_t stroke = s_RenderData.StrokeWeight;

		s_RenderData.FillColour = s_RenderData.StrokeColour;
		s_RenderData.StrokeWeight = 0;

		Rect(x, y, w + stroke * 2, h + stroke * 2);

		s_RenderData.FillColour = fillColour;
		s_RenderData.StrokeWeight = stroke;
	}


	glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(x, y, 0)) * glm::rotate(glm::mat4(1), 0.0f, { 0, 0, 1 }) * glm::scale(glm::mat4(1), glm::vec3(w, h, 0));

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[0];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(0, 0);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[1];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(1, 0);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[2];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(1, 1);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[3];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(0, 1);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectIndexCount += 6;

	s_RenderData.RectCount++;
}

void Renderer::Ellipse(int x, int y, int w, int h)
{
	uint32_t vertexCount = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	if (h == -1)
		h = w;

	if (s_RenderData.StrokeWeight != 0.0f)
	{
		glm::vec4 fillColour(s_RenderData.FillColour);
		uint32_t stroke = s_RenderData.StrokeWeight;

		s_RenderData.FillColour = s_RenderData.StrokeColour;
		s_RenderData.StrokeWeight = 0;

		Ellipse(x, y, w + stroke * 2, h + stroke * 2);

		s_RenderData.FillColour = fillColour;
		s_RenderData.StrokeWeight = stroke;
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(x, y, 0)) * glm::scale(glm::mat4(1), glm::vec3(w, h, 0));

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[0];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[0] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.CircleVerticesCurr->Thickness = 1;
	s_RenderData.CircleVerticesCurr->Fade = 0.02;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[1];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[1] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.CircleVerticesCurr->Thickness = 1;
	s_RenderData.CircleVerticesCurr->Fade = 0.02;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[2];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[2] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.CircleVerticesCurr->Thickness = 1;
	s_RenderData.CircleVerticesCurr->Fade = 0.02;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[3];
	s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[3] * 2.0f;
	s_RenderData.CircleVerticesCurr->Colour = s_RenderData.FillColour;
	s_RenderData.CircleVerticesCurr->Thickness = 1;
	s_RenderData.CircleVerticesCurr->Fade = 0.02;
	s_RenderData.CircleVerticesCurr++;

	s_RenderData.CircleIndexCount += 6;

	s_RenderData.RectCount++;
}

void Renderer::Line(int x1, int y1, int x2, int y2)
{
	uint32_t vertexCount = s_RenderData.RectVerticesCurr - s_RenderData.RectVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glm::vec2 center((x1 + x2) / 2.0f, (y1 + y2) / 2.0f);
	float angle = glm::atan((float)(y2 - y1) / (x2 - x1));

	glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(center.x, center.y, 0))
		* glm::rotate(glm::mat4(1), angle, { 0, 0, 1 })
		* glm::scale(glm::mat4(1), glm::vec3(glm::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)), s_RenderData.StrokeWeight, 0));

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[0];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.StrokeColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(0, 0);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[1];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.StrokeColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(1, 0);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[2];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.StrokeColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(1, 1);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[3];
	s_RenderData.RectVerticesCurr->Colour = s_RenderData.StrokeColour;
	s_RenderData.RectVerticesCurr->TexCoords = glm::vec2(0, 1);
	s_RenderData.RectVerticesCurr->TexIndex = 1;
	s_RenderData.RectVerticesCurr++;

	s_RenderData.RectIndexCount += 6;

	s_RenderData.RectCount++;
}

uint32_t Renderer::QuadCount()
{
	return s_RenderData.RectCount;
}

uint32_t Renderer::DrawCalls()
{
	return s_RenderData.DrawCalls;
}

Shader* Renderer::RectShader()
{
	return s_RenderData.RectShader;
}

Shader* Renderer::CircleShader()
{
	return s_RenderData.CircleShader;
}

Shader* Renderer::LineShader()
{
	return s_RenderData.LineShader;
}

void Renderer::Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	s_RenderData.FillColour = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void Renderer::Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	s_RenderData.StrokeColour = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void Renderer::StrokeWeight(uint32_t weight)
{
	s_RenderData.StrokeWeight = weight;
}