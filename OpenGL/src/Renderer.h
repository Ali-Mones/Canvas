#pragma once
#include "Shader.h"
#include "Vertex.h"
#include "Shape.h"

static class Renderer
{
public:
	static void Init();
	static void BatchStart();
	static void BatchEnd();
	static void Flush();
	static void Shutdown();

	static void Clear(glm::vec4 colour);
	static void RenderQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation = 0.0f);
	static void RenderQuad(Quad quad) { RenderQuad(quad.Position, quad.Dimensions, quad.Colour, quad.Rotation); };
};

