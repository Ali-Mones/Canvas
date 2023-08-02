#pragma once
#include "Shader.h"
#include "Vertex.h"
#include "Shape.h"

class Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void StartBatch();
	static void Flush();

	static void Clear(glm::vec4 colour);

	static void RenderQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation, float stroke = 0.0f);
	static void RenderQuad(Quad quad) { RenderQuad(quad.Position, quad.Dimensions, quad.Colour, quad.Rotation); };

	static void RenderCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness, float fade, float stroke = 0.0f);
	static void RenderCircle(Circle circle) { RenderCircle(circle.Position, circle.Dimensions, circle.Colour, circle.Thickness, circle.Fade); }

	static void RenderLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight);
	static void RenderLine(Line line) { RenderLine(line.p1, line.p2, line.colour, line.weight); }

	static uint32_t QuadCount();
	static uint32_t DrawCalls();
	static Shader* QuadShader();
	static Shader* CircleShader();
	static Shader* LineShader();
};
