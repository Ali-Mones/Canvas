#pragma once
#include "Shader.h"
#include "Vertex.h"
#include "Shape.h"

struct RenderStats;

static class Renderer
{
public:
	static void Init();
	static void BatchStart();
	static void BatchEnd();
	static void Flush();
	static void Shutdown();

	static void Clear(glm::vec4 colour);

	static void RenderQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation);
	static void RenderQuad(Quad quad) { RenderQuad(quad.Position, quad.Dimensions, quad.Colour, quad.Rotation); };

	static void RenderCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness, float fade);
	static void RenderCircle(Circle circle) { RenderCircle(circle.Position, circle.Dimensions, circle.Colour, circle.Thickness, circle.Fade); };

	static uint32_t QuadCount();
	static uint32_t DrawCalls();
	static Shader* QuadShader();
	static Shader* CircleShader();
};
