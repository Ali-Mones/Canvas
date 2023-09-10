#include "Renderer.h"

#include <iostream>
#include <glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stb_image_write/stb_image_write.h>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Vertex.h"
#include "Texture.h"

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
	Texture* FontAtlas;
	std::unordered_map<char, FontCharacter> CharactersMap;

	glm::vec4 UnitRectVertices[4];

	/* Statistics */
	uint32_t RectCount = 0;
	uint32_t DrawCalls = 0;

	static const uint32_t MaxRectCount = 500;
	static const uint32_t MaxVertexCount = MaxRectCount * 4;
	static const uint32_t MaxIndexCount = MaxRectCount * 6;
	static const uint32_t MaxTextureSlots = 32;	// TODO: check how to make dynamic

	const Texture* TextureSlots[32];
	uint32_t TextureSlotIndex = 1; // index 0 = white texture
	Texture* WhiteTexture;
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

	s_RenderData.WhiteTexture = new Texture();
	uint32_t whiteData = 0xffffffff;
	s_RenderData.WhiteTexture->SetData(&whiteData, sizeof(uint32_t));
	s_RenderData.TextureSlots[0] = s_RenderData.WhiteTexture;

	int samplers[s_RenderData.MaxTextureSlots];
	for (int i = 0; i < s_RenderData.MaxTextureSlots; i++)
		samplers[i] = i;
	
	s_RenderData.RectShader->SetUniformArray1i("u_Textures", samplers, s_RenderData.MaxTextureSlots);

	// Text initialisation
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Could not init FreeType Library\n";
		return;
	}

	FT_Face face;
	std::string filepath = "../Canvas/res/fonts/Arial.ttf";
	if (FT_New_Face(ft, filepath.c_str(), 0, &face))
	{
		std::cout << "Could not load font " << filepath << std::endl;
		return;
	}

	uint32_t fontSize = 512;
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	uint32_t textureWidth = 0;
	uint32_t textureHeight = 0;

	int chars = 128;
	int base = 0;

	for (int c = base; c < base + chars; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Could not load character " << c << std::endl;
			continue;
		}

		textureWidth += face->glyph->bitmap.width;
		textureHeight = std::max(textureHeight, face->glyph->bitmap.rows);
	}

	uint8_t* buffer = new uint8_t[textureWidth * textureHeight + 10 * 128 * textureHeight];
	memset(buffer, 0, textureWidth * textureHeight + 10 * 128 * textureHeight);
	uint32_t offset = 0;
	for (int c = base; c < base + chars; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Could not load character " << c << std::endl;
			continue;
		}

		FontCharacter fc =
		{
			.Offset = offset,
			.Size = { face->glyph->bitmap.width, face->glyph->bitmap.rows },
			.Bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top },
			.Advance = (uint32_t)face->glyph->advance.x
		};

		s_RenderData.CharactersMap[c] = fc;

		for (int i = 0; i < face->glyph->bitmap.rows; i++)
			memcpy(&buffer[i * textureWidth + offset], face->glyph->bitmap.buffer + i * face->glyph->bitmap.width, face->glyph->bitmap.width);

		offset += face->glyph->bitmap.width;
		offset += 10; // add space between characters
	}

	s_RenderData.FontAtlas = new Texture(buffer, textureWidth, textureHeight, 1);

	s_RenderData.TextShader->SetUniform1i("u_Texture", 0);
	
	delete[] buffer;
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Renderer::StartBatch()
{
	s_RenderData.RectVerticesCurr = s_RenderData.RectVerticesBase;
	s_RenderData.RectIndexCount = 0;

	s_RenderData.CircleVerticesCurr = s_RenderData.CircleVerticesBase;
	s_RenderData.CircleIndexCount = 0;

	s_RenderData.TextVerticesCurr = s_RenderData.TextVerticesBase;
	s_RenderData.TextIndexCount = 0;

	s_RenderData.TextureSlotIndex = 1;
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

	if (s_RenderData.TextVerticesBase != s_RenderData.TextVerticesCurr)
	{
		uint32_t count = s_RenderData.TextVerticesCurr - s_RenderData.TextVerticesBase;
		s_RenderData.TextVertexBuffer->SetBuffer(count * sizeof(TextVertex), s_RenderData.TextVerticesBase);
		s_RenderData.TextVertexArray->Bind();
		s_RenderData.TextShader->Bind();
		s_RenderData.FontAtlas->Bind();
		s_RenderData.RectIndexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, s_RenderData.TextIndexCount, GL_UNSIGNED_INT, nullptr);
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

void Renderer::Clear(glm::vec4 colour)
{
	s_RenderData.DrawCalls = 0;
	s_RenderData.RectCount = 0;

	glClearColor(colour.r, colour.g, colour.b, colour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Rect(glm::vec3 position, glm::vec3 dimensions, float angle, glm::vec4 fillColour, glm::vec4 strokeColour, uint32_t thickness, const Texture* texture, float tilingFactor)
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
		* glm::rotate(glm::mat4(1), angle, { 0, 0, 1 })
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

void Renderer::Ellipse(glm::vec3 position, glm::vec3 dimensions, glm::vec4 fillColour, glm::vec4 strokeColour, int thickness, float angle)
{
	uint32_t vertexCount = s_RenderData.CircleVerticesCurr - s_RenderData.CircleVerticesBase;
	if (vertexCount == s_RenderData.MaxVertexCount)
	{
		Flush();
		StartBatch();
	}

	glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(position.x, position.y, position.z))
		* glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1))
		* glm::scale(glm::mat4(1), glm::vec3(dimensions.x, dimensions.y, 0));

	float r = dimensions.x > dimensions.y ? 1.0f - thickness / dimensions.y : 1.0f - thickness / dimensions.x;

	if (thickness == -1)
		r = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		s_RenderData.CircleVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[i];
		s_RenderData.CircleVerticesCurr->LocalPosition = s_RenderData.UnitRectVertices[i] * 2.0f;
		s_RenderData.CircleVerticesCurr->FillColour = fillColour;
		s_RenderData.CircleVerticesCurr->StrokeColour = strokeColour;
		s_RenderData.CircleVerticesCurr->Thickness = r;
		s_RenderData.CircleVerticesCurr++;
	}

	s_RenderData.CircleIndexCount += 6;

	s_RenderData.RectCount++;
}

void Renderer::Text(const glm::vec3& position, float angle, const glm::vec4& colour, const std::string& text, uint32_t fontSize)
{
	float scale = fontSize / 512.0f;
	float width = 0;
	float height = 0;
	float maxBearing = 0;
	float maxUnderside = 0;
	for (auto c : text)
	{
		FontCharacter& fc = s_RenderData.CharactersMap[c];
		width += fc.Advance / 64 * scale;
		maxBearing = std::max(maxBearing, fc.Bearing.y * scale);
		maxUnderside = std::max(maxUnderside, scale * (fc.Size.y - fc.Bearing.y));
	}

	height = maxBearing + maxUnderside;

	float x = 0;
	float y = 0;

	for (auto c : text)
	{
		uint32_t vertexCount = s_RenderData.TextVerticesCurr - s_RenderData.TextVerticesBase;
		if (vertexCount == s_RenderData.MaxVertexCount)
		{
			Flush();
			StartBatch();
		}

		FontCharacter& fc = s_RenderData.CharactersMap[c];
		float xpos = x + fc.Bearing.x * scale + fc.Size.x * scale / 2;
		float ypos = y + fc.Bearing.y * scale - fc.Size.y * scale / 2;

		glm::mat4 transform = glm::translate(glm::mat4(1), glm::vec3(position.x + xpos - width / 2, position.y + ypos - height / 2, position.z))
			* glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1), glm::vec3(fc.Size.x * scale, fc.Size.y * scale, 0));


		s_RenderData.TextVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[0];
		s_RenderData.TextVerticesCurr->Colour = colour;
		s_RenderData.TextVerticesCurr->TexCoords = { (float)fc.Offset / s_RenderData.FontAtlas->Width(), 1.0f - (float)fc.Size.y / s_RenderData.FontAtlas->Height() };
		s_RenderData.TextVerticesCurr++;

		s_RenderData.TextVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[1];
		s_RenderData.TextVerticesCurr->Colour = colour;
		s_RenderData.TextVerticesCurr->TexCoords = { (float)(fc.Offset + fc.Size.x) / s_RenderData.FontAtlas->Width(), 1.0f - (float)fc.Size.y / s_RenderData.FontAtlas->Height() };
		s_RenderData.TextVerticesCurr++;

		s_RenderData.TextVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[2];
		s_RenderData.TextVerticesCurr->Colour = colour;
		s_RenderData.TextVerticesCurr->TexCoords = { (float)(fc.Offset + fc.Size.x) / s_RenderData.FontAtlas->Width(), 1.0f };
		s_RenderData.TextVerticesCurr++;

		s_RenderData.TextVerticesCurr->Position = transform * s_RenderData.UnitRectVertices[3];
		s_RenderData.TextVerticesCurr->Colour = colour;
		s_RenderData.TextVerticesCurr->TexCoords = { (float)fc.Offset / s_RenderData.FontAtlas->Width(), 1.0f };
		s_RenderData.TextVerticesCurr++;

		x += fc.Advance * scale / 64;

		s_RenderData.TextIndexCount += 6;
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
