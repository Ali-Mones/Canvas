#pragma once
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec4 Position;
	glm::vec4 Colour;
	glm::vec2 TextureCoords;
	float TextureIndex;

	Vertex() = default;
	Vertex(glm::vec4 Position, glm::vec4 Colour, glm::vec2 TextureCoords, int TextureIndex)
	: Position(Position), Colour(Colour), TextureCoords(TextureCoords), TextureIndex(TextureIndex)
	{
	}
};