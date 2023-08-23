#include <vector>
#include <glm/gtx/compatibility.hpp>

#include "Renderer.h"
#include "Canvas.h"

struct CanvasData
{
	/* Attributes */
	glm::vec4 FillColour = glm::vec4(1);
	glm::vec4 StrokeColour = glm::vec4(1);
	uint32_t StrokeWeight = 0;
	bool Filled = true;
};

static CanvasData s_Data;

namespace Canvas {

	void Clear(uint32_t r, uint32_t g, uint32_t b)
	{
		Renderer::Clear(glm::vec4(r / 255, g / 255, b / 255, 1));
	}

	void Rect(int x, int y, uint32_t w, uint32_t h)
	{
		glm::vec3 pos = glm::vec3(x, y, 0);
		glm::vec3 dims = glm::vec3(w, h, 0);

		glm::vec4 fillColour = glm::vec4(s_Data.FillColour.r, s_Data.FillColour.g, s_Data.FillColour.b, s_Data.FillColour.a);
		if (!s_Data.Filled)
			fillColour = glm::vec4(0);

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeWeight ? s_Data.StrokeColour.a : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Rect(pos, dims, fillColour, strokeColour, s_Data.StrokeWeight, 0);
	}

	void Ellipse(int x, int y, int w, int h)
	{
		glm::vec3 pos = glm::vec3(x, y, 0);
		glm::vec3 dims = glm::vec3(w, h, 0);

		glm::vec4 fillColour = glm::vec4(s_Data.FillColour.r, s_Data.FillColour.g, s_Data.FillColour.b, s_Data.FillColour.a);
		if (!s_Data.Filled)
			fillColour = glm::vec4(0);

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeColour.a);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Ellipse(pos, dims, fillColour, -1, 0);
		Renderer::Ellipse(pos, dims, strokeColour, s_Data.StrokeWeight, 0);
	}

	void Line(int x1, int y1, int x2, int y2)
	{
		glm::vec3 p1(x1, y1, 0);
		glm::vec3 p2(x2, y2, 0);
		glm::vec3 pos((p1 + p2) / 2.0f);
		glm::vec3 dims(glm::distance(p1, p2), s_Data.StrokeWeight, 0);
		float angle = glm::atan((float)(y2 - y1) / (x2 - x1));

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeWeight ? s_Data.StrokeColour.a : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Rect(pos, dims, strokeColour, glm::vec4(0), 0, angle);

		float diameter = s_Data.StrokeWeight;
		glm::vec3 circleDims(diameter, diameter, 0);

		Renderer::Ellipse(p1, circleDims, strokeColour, -1, 0);
		Renderer::Ellipse(p2, circleDims, strokeColour, -1, 0);
	}

	void BezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		glm::vec2 v1(x1, y1);
		glm::vec2 v2(x2, y2);
		glm::vec2 v3(x3, y3);
		glm::vec2 v4(x4, y4);

		std::vector<glm::vec2> points;

		for (float t = 0.0f; t <= 1.0001f; t += 0.02f)
		{
			glm::vec2 b = (float) glm::pow(1 - t, 3) * v1 + 3 * (float) glm::pow(1 - t, 2) * t * v2 + 3 * (1 - t) * (float) glm::pow(t, 2) * v3 + (float) glm::pow(t, 3) * v4;
			points.push_back(b);
		}

		for (int i = 1; i < points.size(); i++)
		{
			glm::vec2& v1 = points[i - 1];
			glm::vec2& v2 = points[i];
			Line(v1.x, v1.y, v2.x, v2.y);
		}
	}

	void Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		s_Data.Filled = true;
		s_Data.FillColour = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
		s_Data.StrokeColour = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	void StrokeWeight(uint32_t weight)
	{
		s_Data.StrokeWeight = weight;
	}

	void NoFill()
	{
		s_Data.Filled = false;
	}

	void NoStroke()
	{
		s_Data.StrokeWeight = 0;
	}
}
