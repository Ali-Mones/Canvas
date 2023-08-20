#pragma once
#include <glm/glm.hpp>

struct Rect
{
	int x;
	int y;
	int w;
	int h;
	glm::vec4 Colour;
};

struct Circle
{
	int x;
	int y;
	int w;
	int h;
	glm::vec4 Colour;
};

struct Line
{
	glm::vec2 p1;
	glm::vec2 p2;
	glm::vec4 colour;
	float weight;
};
