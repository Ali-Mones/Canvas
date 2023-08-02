#pragma once
#include <glm/glm.hpp>

struct Shape
{
	glm::vec3 Position;
	glm::vec3 Dimensions;
	glm::vec4 Colour;

	Shape(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec4& colour, float rotation = 0.0f)
		: Position(position), Dimensions(dimensions), Colour(colour)
	{
	}
};

struct Quad : public Shape
{
	float Rotation;

	Quad(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec4& colour, float rotation = 0.0f)
		: Shape(position, dimensions, colour), Rotation(rotation)
	{
	}
};

struct Circle : public Shape
{
	float Thickness;
	float Fade;

	Circle(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec4 colour, float thickness = 1.0f, float fade = 0.02f)
		: Shape(position, dimensions, colour), Thickness(thickness), Fade(fade)
	{
	}
};

struct Line
{
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec4 colour;
	float weight;
};
