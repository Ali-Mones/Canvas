#pragma once
#include <glm/glm.hpp>

struct FilledQuadVertex
{
	glm::vec4 Position;
	glm::vec4 Colour;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct HollowQuadVertex
{
	glm::vec4 Position;
	glm::vec2 LocalPosition;
	glm::vec4 Colour;
	float Thickness;
};

struct CircleVertex
{
	glm::vec4 Position;
	glm::vec2 LocalPosition;
	glm::vec4 Colour;
	float Thickness;
	float Fade;
};

struct LineVertex
{
	glm::vec4 Position;
	glm::vec4 Colour;
};
