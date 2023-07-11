#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec4 Position;
	glm::vec4 Colour;
	glm::vec2 TexCoords;
	float TexIndex;

	Vertex(const glm::vec4& Position, const glm::vec4& Colour, const glm::vec2& TexCoords, float TexIndex)
		: Position(Position), Colour(Colour), TexCoords(TexCoords), TexIndex(TexIndex)
	{
	}
};