#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "Core.h"
#include "Texture.h"

class Shader;

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void StartBatch();
	static void Flush();

	static void Clear(glm::vec4 colour);

	static void Rect(glm::vec3 position, glm::vec3 dimensions, float angle, glm::vec4 fillColour, glm::vec4 strokeColour, uint32_t thickness, const Texture* texture, float tilingFactor);
	static void Ellipse(glm::vec3 position, glm::vec3 dimensions, glm::vec4 fillColour, glm::vec4 strokeColour, int thickness, float angle);
	static void Quad(int p1, int p2, int p3, int p4);

	static uint32_t QuadCount();
	static uint32_t DrawCalls();

	static Shader* RectShader();
	static Shader* CircleShader();
};
