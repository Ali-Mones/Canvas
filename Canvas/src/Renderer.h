#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "Core.h"
#include "Texture2D.h"
#include "Font.h"

class Shader;

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void StartBatch();
	static void Flush();

	static void Clear(const glm::vec4& colour);
	static void Rect(const glm::vec3& position, const glm::vec3& dimensions, float angle, const glm::vec4& fillColour, const glm::vec4& strokeColour, uint32_t thickness, const Texture2D* texture, float tilingFactor);
	static void Ellipse(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec4& fillColour, const glm::vec4& strokeColour, int thickness, float angle);
	static void Quad(int p1, int p2, int p3, int p4);
	static void Text(const glm::vec3& position, const glm::vec3& dimensions, float angle, const glm::vec4& colour, const std::string& text, const Font* font, uint32_t fontSize);

	static uint32_t QuadCount();
	static uint32_t DrawCalls();

	static Shader* RectShader();
	static Shader* CircleShader();
	static Shader* TextShader();
};
