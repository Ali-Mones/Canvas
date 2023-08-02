#pragma once
#include <glm/glm.hpp>

struct Quad
{
	glm::vec3 Position;
	glm::vec3 Dimensions;
	glm::vec4 Colour;
	float Rotation;
};

struct Circle
{
	glm::vec3 Position;
	glm::vec3 Dimensions;
	glm::vec4 Colour;
	float Thickness;
	float Fade;
};

struct Line
{
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec4 colour;
	float weight;
};
