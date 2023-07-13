#pragma once
#include <glm/glm.hpp>

struct QuadVertex
{
	glm::vec4 Position;
	glm::vec4 Colour;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct CircleVertex
{
	glm::vec4 Position;
	glm::vec2 LocalPosition;
	glm::vec4 Colour;
	float Thickness = 1.0f;
	float Fade = 0.02f;
};