#pragma once
#include "Shader.h"
#include "Vertex.h"

static class Renderer
{
public:
	static void Init();
	static void BatchStart();
	static void BatchEnd();
	static void Flush();
	static void Shutdown();

	static void Clear();
	static void Quad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation = 0.0f);
};

