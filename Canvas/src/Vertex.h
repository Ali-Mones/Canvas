#pragma once
#include <glm/glm.hpp>

struct RectVertex
{
	glm::vec4 Position;
	glm::vec2 LocalPosition;
	glm::vec4 FillColour;
	glm::vec4 StrokeColour;
	float ThicknessX;
	float ThicknessY;
	glm::vec2 TexCoords;
	float TexIndex;
	float TilingFactor;
};

struct CircleVertex
{
	glm::vec4 Position;
	glm::vec2 LocalPosition;
	glm::vec4 FillColour;
	glm::vec4 StrokeColour;
	float Thickness;
};

struct TextVertex
{
	glm::vec4 Position;
	glm::vec4 Colour;
	glm::vec2 TexCoords;
	float TexIndex;
};
