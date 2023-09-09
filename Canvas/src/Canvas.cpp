#include <iostream>
#include <vector>
#include <glm/gtx/compatibility.hpp>

#include "Renderer.h"
#include "Canvas.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"

struct CanvasData
{
	/* Attributes */
	glm::vec4 FillColour = glm::vec4(1);

	glm::vec4 StrokeColour = glm::vec4(1);
	uint32_t StrokeWeight = 0;

	float Z = -1.0f;

	float TilingFactor = 1.0f;
	bool HorizontalFlip = false;
	bool VerticalFlip = false;

	std::unordered_map<uint32_t, Texture*> TexturesMap;
};

static CanvasData s_Data;
static uint32_t textureSlot = 1;

namespace Canvas {

	void Clear(uint32_t r, uint32_t g, uint32_t b)
	{
		Renderer::Clear(glm::vec4(r / 255, g / 255, b / 255, 1));
		s_Data.Z = -1.0f;
	}

	void Rect(int x, int y, uint32_t w, uint32_t h)
	{
		glm::vec3 pos = glm::vec3(x, y, s_Data.Z += std::numeric_limits<float>::epsilon());
		glm::vec3 dims = glm::vec3(w, h, 0);

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeWeight ? s_Data.StrokeColour.a : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Rect(pos, dims, 0.0f, s_Data.FillColour, strokeColour, s_Data.StrokeWeight, nullptr, 1.0f);
	}

	void TexturedRect(int x, int y, uint32_t w, uint32_t h, CanvasTexture texture)
	{
		float width = w, height = h;
		if (s_Data.HorizontalFlip)
			width = -width;
		if (s_Data.VerticalFlip)
			height = -height;

		glm::vec3 pos = glm::vec3(x, y, s_Data.Z += std::numeric_limits<float>::epsilon());
		glm::vec3 dims = glm::vec3(width, height, 0);

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeWeight ? s_Data.StrokeColour.a : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		if (s_Data.TexturesMap.count(texture))
			Renderer::Rect(pos, dims, 0.0f, s_Data.FillColour, strokeColour, s_Data.StrokeWeight, s_Data.TexturesMap[texture], s_Data.TilingFactor);
		else
			std::cout << "No Texture found with id = " << texture << std::endl;
	}

	void Ellipse(int x, int y, uint32_t w, uint32_t h)
	{
		glm::vec3 pos = glm::vec3(x, y, s_Data.Z += std::numeric_limits<float>::epsilon());
		glm::vec3 dims = glm::vec3(w, h, 0);

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeColour.a);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		if (strokeColour == glm::vec4(0))
			Renderer::Ellipse(pos, dims, s_Data.FillColour, strokeColour, -1, 0);
		else
			Renderer::Ellipse(pos, dims, s_Data.FillColour, strokeColour, s_Data.StrokeWeight, 0);
	}

	void Line(int x1, int y1, int x2, int y2)
	{
		s_Data.Z += std::numeric_limits<float>::epsilon();

		glm::vec3 p1(x1, y1, s_Data.Z);
		glm::vec3 p2(x2, y2, s_Data.Z);
		glm::vec3 pos((p1 + p2) / 2.0f);
		glm::vec3 dims(glm::distance(p1, p2), s_Data.StrokeWeight, 0);
		float angle = glm::atan((float)(y2 - y1) / (x2 - x1));

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.r, s_Data.StrokeColour.g, s_Data.StrokeColour.b, s_Data.StrokeWeight ? s_Data.StrokeColour.a : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Rect(pos, dims, angle, strokeColour, glm::vec4(0), 0.0f, nullptr, 1.0f);

		float diameter = s_Data.StrokeWeight;
		glm::vec3 circleDims(diameter, diameter, 0);

		Renderer::Ellipse(p1, circleDims, strokeColour, glm::vec4(0), -1, 0);
		Renderer::Ellipse(p2, circleDims, strokeColour, glm::vec4(0), -1, 0);
	}

	void BezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
	{
		s_Data.Z += std::numeric_limits<float>::epsilon();

		glm::vec3 v1(x1, y1, s_Data.Z);
		glm::vec3 v2(x2, y2, s_Data.Z);
		glm::vec3 v3(x3, y3, s_Data.Z);
		glm::vec3 v4(x4, y4, s_Data.Z);

		std::vector<glm::vec3> points;

		for (float t = 0.0f; t <= 1.0001f; t += 0.02f)
		{
			glm::vec3 b = (float) glm::pow(1 - t, 3) * v1 + 3 * (float) glm::pow(1 - t, 2) * t * v2 + 3 * (1 - t) * (float) glm::pow(t, 2) * v3 + (float) glm::pow(t, 3) * v4;
			points.push_back(b);
		}

		for (int i = 1; i < points.size(); i++)
		{
			glm::vec3& v1 = points[i - 1];
			glm::vec3& v2 = points[i];
			Line(v1.x, v1.y, v2.x, v2.y);
		}
	}

	void Point(int x, int y)
	{
		Rect(x, y, 2, 2);
	}

	CanvasTexture CreateTexture(const char* filepath)
	{
		Texture* t = new Texture(filepath);
		s_Data.TexturesMap[t->RendererID()] = t;
		return t->RendererID();
	}

	void Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
	{
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
		s_Data.FillColour = glm::vec4(0);
	}

	void NoStroke()
	{
		s_Data.StrokeWeight = 0;
	}

	void TextureTilingFactor(float tilingFactor)
	{
		s_Data.TilingFactor = tilingFactor;
	}

	void NoTextureTiling()
	{
		s_Data.TilingFactor = 1.0f;
	}

	void FlipTextureHorizontally(bool value)
	{
		s_Data.HorizontalFlip = value;
	}

	void FlipTextureVertically(bool value)
	{
		s_Data.VerticalFlip = value;
	}

	double MouseX()
	{
		double xpos, ypos;
		GLFWwindow* window = Application::Get().GetWindow()->NativeWindow();
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec4 pos = Camera::Get().GetWorldPosition(xpos, ypos);
		return pos.x;
	}

	double MouseY()
	{
		double xpos, ypos;
		GLFWwindow* window = Application::Get().GetWindow()->NativeWindow();
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec4 pos = Camera::Get().GetWorldPosition(xpos, ypos);
		return pos.y;
	}

	uint32_t WindowWidth()
	{
		return Camera::Get().WindowWidth();
	}

	uint32_t WindowHeight()
	{
		return Camera::Get().WindowHeight();
	}

	void Shutdown()
	{
		for (auto texture : s_Data.TexturesMap)
			delete texture.second;
	}
}
