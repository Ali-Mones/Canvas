#include "Renderer.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stb_image_write/stb_image_write.h>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "Texture2D.h"
#include "Font.h"

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <msdf-atlas-gen.h>

struct FontCharacter
{
	uint32_t Offset;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	uint32_t Advance;
};

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

	uint32_t TextIndexCount = 0;
	VertexArray* TextVertexArray;
	VertexBuffer* TextVertexBuffer;
	TextVertex* TextVerticesBase;
	TextVertex* TextVerticesCurr;
	Shader* TextShader;

	const Font* FontTextureSlots[32];
	uint32_t FontTextureSlotIndex = 0;

	glm::vec4 UnitRectVertices[4];

	/* Statistics */
	uint32_t RectCount = 0;
	uint32_t DrawCalls = 0;

	static const uint32_t MaxRectCount = 500;
	static const uint32_t MaxVertexCount = MaxRectCount * 4;
	static const uint32_t MaxIndexCount = MaxRectCount * 6;
	static const uint32_t MaxTextureSlots = 32;	// TODO: check how to make dynamic

	const Texture2D* TextureSlots[32];
	uint32_t TextureSlotIndex = 1; // index 0 = white texture
	Texture2D* WhiteTexture;
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

	// Text initialisation
	s_RenderData.TextShader = new Shader("res/shaders/Text.shader");
	s_RenderData.TextVertexArray = new VertexArray();
	s_RenderData.TextVertexBuffer = new VertexBuffer(s_RenderData.MaxVertexCount * sizeof(TextVertex));
	s_RenderData.TextVertexArray->SetLayout<TextVertex>();
	s_RenderData.TextVerticesBase = new TextVertex[s_RenderData.MaxVertexCount];
	s_RenderData.TextVerticesCurr = s_RenderData.TextVerticesBase;

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

	s_RenderData.WhiteTexture = new Texture2D();
	uint32_t whiteData = 0xffffffff;
	s_RenderData.WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
	s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;

	int samplers[s_RenderData.MaxTextureSlots];
	for (int i = 0; i < s_RenderData.MaxTextureSlots; i++)
		samplers[i] = i;
	
	s_RenderData.RectShader->SetUniformArray1i("u_Textures", samplers, s_RenderData.MaxTextureSlots);
	s_RenderData.TextShader->SetUniformArray1i("u_Textures", samplers, s_RenderData.MaxTextureSlots);
}

void Renderer::StartBatch()
{
	s_RenderData.RectVerticesCurr = s_RenderData.RectVerticesBase;
	s_RenderData.RectIndexCount = 0;
	s_RenderData.TextureSlotIndex = 1;

	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;
	s_RenderData.CircleIndexCount = 0;

	s_RenderData.TextVerticesCurr = s_RenderData.TextVerticesBase;
	s_RenderData.TextIndexCount = 0;
	s_RenderData.FontTextureSlotIndex = 0;
}

void Renderer::Flush()
{
	if (s_RenderData.RectVerticesBase != s_RenderData.RectVerticesCurr)
	{
		uint32_t count = s_RenderData.RectVerticesCurr - s_RenderData.RectVerticesBase;
		s_RenderData.RectVertexBuffer->SetBuffer(count * sizeof(RectVertex), s_RenderData.RectVerticesBase);

		for (uint32_t i = 0; i < s_RenderData.TextureSlotIndex; i++)
			s_RenderData.TextureSlots[i]->Bind(i);

		s_RenderData.RectVertexArray->Bind();
		s_RenderData.RectShader->Bind();
		s_RenderData.RectIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.RectIndexCount, GL_UNSIGNED_INT, nullptr);
		s_RenderData.DrawCalls++;
	}

	if (s_RenderData.TextVerticesBase != s_RenderData.TextVerticesCurr)
	{
		uint32_t count = s_RenderData.TextVerticesCurr - s_RenderData.TextVerticesBase;
		s_RenderData.TextVertexBuffer->SetBuffer(count * sizeof(TextVertex), s_RenderData.TextVerticesBase);

		for (uint32_t i = 0; i < s_RenderData.FontTextureSlotIndex; i++)
			s_RenderData.FontTextureSlots[i]->FontAtlas->Bind(i);

		s_RenderData.TextVertexArray->Bind();
		s_RenderData.TextShader->Bind();
		s_RenderData.RectIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.TextIndexCount, GL_UNSIGNED_INT, nullptr);
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

	delete s_RenderData.TextVertexArray;
	delete s_RenderData.TextVertexBuffer;
	delete s_RenderData.TextShader;
	delete s_RenderData.TextVerticesBase;

	delete s_RenderData.WhiteTexture;
}

void Renderer::Clear(const glm::vec4& colour)
{
	s_RenderData.DrawCalls = 0;
	s_RenderData.RectCount = 0;

	glClearColor(colour.r, colour.g, colour.b, colour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Rect(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation, const glm::vec4& fillColour, const glm::vec4& strokeColour, uint32_t thickness, const Texture2D* texture, float tilingFactor)
{
	uint32_t vertexCount = s_RenderData.RectVerticesCurr - s_RenderData.RectVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	float textureIndex = 0.0f;
	if (texture)
	{
		if (s_RenderData.TextureSlotIndex >= s_RenderData.MaxTextureSlots)
		{
			Flush();
			StartBatch();
		}

		for (int i = 1; i < s_RenderData.TextureSlotIndex; i++)
		{
			if (*s_RenderData.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex++] = texture;
		}
	}

	float thicknessX = (float) thickness / dimensions.x;
	float thicknessY = (dimensions.x / dimensions.y) * thicknessX;

	glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, position.z))
		* glm::toMat4(glm::quat(rotation))
		* glm::scale(glm::mat4(1), glm::vec3(dimensions.x, dimensions.y, 0));

	static glm::vec2 texCoords[] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

	for (int i = 0; i < 4; i++)
	{
		s_RenderData.RectVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[i];
		s_RenderData.RectVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[i] * 2.0f;
		s_RenderData.RectVerticesCurr->FillColour = fillColour;
		s_RenderData.RectVerticesCurr->StrokeColour = strokeColour;
		s_RenderData.RectVerticesCurr->ThicknessX = thicknessX;
		s_RenderData.RectVerticesCurr->ThicknessY = thicknessY;
		s_RenderData.RectVerticesCurr->TexCoords = texCoords[i];
		s_RenderData.RectVerticesCurr->TexIndex = textureIndex;
		s_RenderData.RectVerticesCurr->TilingFactor = tilingFactor;
		s_RenderData.RectVerticesCurr++;
	}

	s_RenderData.RectIndexCount += 6;

	s_RenderData.RectCount++;
}

void Renderer::Ellipse(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation, const glm::vec4& fillColour, const glm::vec4& strokeColour, int thickness)
{
	uint32_t vertexCount = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, position.z))
		* glm::toMat4(glm::quat(rotation))
		* glm::scale(glm::mat4(1), glm::vec3(dimensions.x, dimensions.y, 0));

	float thiccness = dimensions.x > dimensions.y ? thickness / dimensions.y : thickness / dimensions.x;
	thiccness *= 2.0f;

	for (int i = 0; i < 4; i++)
	{
		s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[i];
		s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[i] * 2.0f;
		s_RenderData.CircleVerticesCurr->FillColour = fillColour;
		s_RenderData.CircleVerticesCurr->StrokeColour = strokeColour;
		s_RenderData.CircleVerticesCurr->Thickness = thiccness;
		s_RenderData.CircleVerticesCurr++;
	}

	s_RenderData.CircleIndexCount += 6;

	s_RenderData.RectCount++;
}

void Renderer::Text(const glm::vec3& position, const glm::vec3& rotation, const glm::vec4& colour, const std::string& text, const Font* font, uint32_t fontSize)
{
	assert(font);
	uint32_t vertexCount = s_RenderData.TextVerticesCurr - s_RenderData.TextVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount || s_RenderData.FontTextureSlotIndex >= s_RenderData.MaxTextureSlots)
	{
		Flush();
		StartBatch();
	}

	float textureIndex = -1.0f;
	for (int i = 0; i < s_RenderData.FontTextureSlotIndex; i++)
	{
		if (font->FontAtlas == s_RenderData.FontTextureSlots[i]->FontAtlas)
		{
			textureIndex = i;
			break;
		}
	}

	if (textureIndex == -1.0f)
	{
		textureIndex = s_RenderData.FontTextureSlotIndex;
		s_RenderData.FontTextureSlots[s_RenderData.FontTextureSlotIndex++] = font;
	}

	const auto& metrics = font->FontGeometry.getMetrics();

	double fsScale = 1 / (metrics.ascenderY - metrics.descenderY);
	double x = 0, y = 0;
	double texelWidth = 1.0 / font->FontAtlas->Width();
	double texelHeight = 1.0 / font->FontAtlas->Height();

	for (const char& c : text)
	{
		const msdf_atlas::GlyphGeometry* glyph = font->FontGeometry.getGlyph(c);
		if (glyph)
		{
			if (!glyph->isWhitespace())
			{
				double pl, pb, pr, pt;
				double il, ib, ir, it;
				glyph->getQuadPlaneBounds(pl, pb, pr, pt);
				glyph->getQuadAtlasBounds(il, ib, ir, it);
				pl *= fsScale, pb *= fsScale, pr *= fsScale, pt *= fsScale;
				pl += x, pb += y, pr += x, pt += y;
				il *= texelWidth, ib *= texelHeight, ir *= texelWidth, it *= texelHeight;

				glm::mat4 transform = glm::translate(glm::mat4(1), position)
					* glm::toMat4(glm::quat(rotation))
					* glm::scale(glm::mat4(1), glm::vec3(fontSize, fontSize, 0));

				s_RenderData.TextVerticesCurr->Position = transform * glm::vec4(pl, pb, 0.0f, 1.0f);
				s_RenderData.TextVerticesCurr->Colour = colour;
				s_RenderData.TextVerticesCurr->TexCoords = { il, ib };
				s_RenderData.TextVerticesCurr->TexIndex = textureIndex;
				s_RenderData.TextVerticesCurr++;

				s_RenderData.TextVerticesCurr->Position = transform * glm::vec4(pr, pb, 0.0f, 1.0f);
				s_RenderData.TextVerticesCurr->Colour = colour;
				s_RenderData.TextVerticesCurr->TexCoords = { ir, ib };
				s_RenderData.TextVerticesCurr->TexIndex = textureIndex;
				s_RenderData.TextVerticesCurr++;

				s_RenderData.TextVerticesCurr->Position = transform * glm::vec4(pr, pt, 0.0f, 1.0f);
				s_RenderData.TextVerticesCurr->Colour = colour;
				s_RenderData.TextVerticesCurr->TexCoords = { ir, it };
				s_RenderData.TextVerticesCurr->TexIndex = textureIndex;
				s_RenderData.TextVerticesCurr++;

				s_RenderData.TextVerticesCurr->Position = transform * glm::vec4(pl, pt, 0.0f, 1.0f);
				s_RenderData.TextVerticesCurr->Colour = colour;
				s_RenderData.TextVerticesCurr->TexCoords = { il, it };
				s_RenderData.TextVerticesCurr->TexIndex = textureIndex;
				s_RenderData.TextVerticesCurr++;
			}

			double advance = glyph->getAdvance();
			font->FontGeometry.getAdvance(advance, c, *((&c) + 1));
			x += fsScale * advance;

			s_RenderData.TextIndexCount += 6;
		}
	}
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

Shader* Renderer::TextShader()
{
	return s_RenderData.TextShader;
}
