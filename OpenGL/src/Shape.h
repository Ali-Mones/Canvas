#pragma once
#include <glm/glm.hpp>

struct Shape
{
	glm::vec3 Position;
	glm::vec3 Dimensions;
	glm::vec4 Colour;
	float Rotation;

	Shape(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec4& colour, float rotation = 0.0f)
		: Position(position), Dimensions(dimensions), Colour(colour), Rotation(rotation)
	{
	}
};

struct Quad : public Shape
{
	Quad(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec4& colour, float rotation = 0.0f)
		: Shape(position, dimensions, colour, rotation)
	{
	}
};

