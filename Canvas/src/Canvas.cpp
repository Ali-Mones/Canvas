#include <iostream>
#include <vector>
#include <algorithm>
#include <glm/gtx/compatibility.hpp>

#include "Renderer.h"
#include "Canvas.h"
#include "Window.h"
#include "Camera.h"
#include "Texture2D.h"
#include "Font.h"
#include "Sound.h"

struct CanvasData
{
	/* Attributes */
	Canvas::PositionMode PositionMode = Canvas::PositionMode::TopLeft;
	Canvas::OriginPosition OriginPosition = Canvas::OriginPosition::TopLeft;

	Canvas::Vector4 FillColour = Canvas::Vector4(1.0f);

	Canvas::Vector4 StrokeColour = Canvas::Vector4(Canvas::Vector3(0.0f), 1.0f);
	float StrokeWeight = 1;

	float Z = -1.0f;
	Canvas::Vector3 Rotation = Canvas::Vector3(0.0f);

	float TilingFactor = 1.0f;
	bool HorizontalFlip = false;
	bool VerticalFlip = false;

	float FontSize = 18;

	std::unordered_map<uint32_t, Texture2D*> TexturesMap;
	std::unordered_map<uint32_t, Font*> FontsMap;
	std::unordered_map<uint32_t, Sound*> SoundsMap;
};

static CanvasData s_Data;

static Canvas::Vector3 GetTextDimensions(const std::string& text, Font* font)
{
	const auto& metrics = font->FontGeometry.getMetrics();

	double fsScale = 1 / (metrics.ascenderY - metrics.descenderY);
	double x = 0, y = 0;
	double textWidth = 0.0;

	for (const char& c : text)
	{
		const msdf_atlas::GlyphGeometry* glyph = font->FontGeometry.getGlyph(c);
		if (glyph)
		{
			double advance = glyph->getAdvance();
			font->FontGeometry.getAdvance(advance, c, *((&c) + 1));
			x += fsScale * advance;
		}
	}
	textWidth = x;
	double textHeight = fsScale * metrics.lineHeight;

	return Canvas::Vector3(textWidth, textHeight, 0.0f);
}

static Canvas::Vector3 StandardisePosition(const Canvas::Vector3& position, const Canvas::Vector3& dimensions)
{
	Canvas::Vector3 pos = position;
	if (s_Data.OriginPosition == Canvas::OriginPosition::Center)
	{
		pos.x += Canvas::WindowWidth() / 2;
		pos.y += Canvas::WindowHeight() / 2;
	}
	else if (s_Data.OriginPosition == Canvas::OriginPosition::BottomRight)
		pos.x = Canvas::WindowWidth() - pos.x;
	else if (s_Data.OriginPosition == Canvas::OriginPosition::TopLeft)
		pos.y = Canvas::WindowHeight() - pos.y;
	else if (s_Data.OriginPosition == Canvas::OriginPosition::TopRight)
	{
		pos.x = Canvas::WindowWidth() - pos.x;
		pos.y = Canvas::WindowHeight() - pos.y;
	}

	if (s_Data.PositionMode == Canvas::PositionMode::TopLeft)
	{
		pos.x += dimensions.x / 2;
		pos.y -= dimensions.y / 2;
	}
	else if (s_Data.PositionMode == Canvas::PositionMode::BottomLeft)
	{
		pos.x += dimensions.x / 2;
		pos.y += dimensions.y / 2;
	}
	else if (s_Data.PositionMode == Canvas::PositionMode::TopRight)
	{
		pos.x -= dimensions.x / 2;
		pos.y -= dimensions.y / 2;
	}
	else if (s_Data.PositionMode == Canvas::PositionMode::BottomRight)
	{
		pos.x -= dimensions.x / 2;
		pos.y += dimensions.y / 2;
	}

	return pos;
}

namespace Canvas {

	void SetPositionMode(PositionMode mode)
	{
		s_Data.PositionMode = mode;
	}

	void SetOriginPosition(OriginPosition position)
	{
		s_Data.OriginPosition = position;
	}

	void Clear(const Vector3& colour)
	{
		Renderer::Clear(glm::vec4(colour.x / 255.0f, colour.y / 255.0f, colour.z / 255.0f, 1));
		s_Data.Z = -1.0f;
	}

	void Rect(const Vector3& position, const Vector3& dimensions)
	{
		Vector3 pos = StandardisePosition(position, dimensions);
		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.x, s_Data.StrokeColour.y, s_Data.StrokeColour.z, s_Data.StrokeWeight ? s_Data.StrokeColour.w : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Rect(pos, { dimensions.x, dimensions.y, dimensions.z }, s_Data.Rotation, s_Data.FillColour, strokeColour, s_Data.StrokeWeight, nullptr, 1.0f);
	}

	void Rect(const Vector2& position, const Vector2& dimensions)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Vector3 pos = { position, z };
		Vector3 dims = { dimensions, 0.0f };
		Rect(pos, dims);
	}

	void Rect(float x, float y, float w, float h)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Vector3 pos = { x, y, z };
		Vector3 dims = { w, h, 0.0f };
		Rect(pos, dims);
	}

	void TexturedRect(const Vector3& position, const Vector3& dimensions, Texture texture)
	{
		Vector3 pos = StandardisePosition(position, dimensions);

		float w = dimensions.x, h = dimensions.y;
		if (s_Data.HorizontalFlip)
			w = -w;
		if (s_Data.VerticalFlip)
			h = -h;

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.x, s_Data.StrokeColour.y, s_Data.StrokeColour.z, s_Data.StrokeWeight ? s_Data.StrokeColour.w : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		if (s_Data.TexturesMap.count(texture))
			Renderer::Rect(pos, { w, h, 0.0f }, s_Data.Rotation, s_Data.FillColour, strokeColour, s_Data.StrokeWeight, s_Data.TexturesMap[texture], s_Data.TilingFactor);
		else
			std::cout << "No Texture found with id = " << texture << std::endl;
	}

	void TexturedRect(const Vector2& position, const Vector2& dimensions, Texture texture)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Vector3 pos = { position, z };
		Vector3 dims = { dimensions, 0.0f };
		TexturedRect(pos, dims, texture);
	}

	void TexturedRect(float x, float y, float w, float h, Texture texture)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Vector3 pos = { x, y, z };
		Vector3 dims = { w, h, 0.0f };
		TexturedRect(pos, dims, texture);
	}

	void Ellipse(const Vector3& position, const Vector3& dimensions)
	{
		Vector3 pos = StandardisePosition(position, dimensions);
		glm::vec4 strokeColour = s_Data.StrokeColour;
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		if (strokeColour == glm::vec4(0))
			Renderer::Ellipse(pos, { dimensions.x, dimensions.y, dimensions.z }, s_Data.Rotation, s_Data.FillColour, strokeColour, 0.0f);
		else
			Renderer::Ellipse(pos, { dimensions.x, dimensions.y, dimensions.z }, s_Data.Rotation, s_Data.FillColour, strokeColour, s_Data.StrokeWeight);
	}

	void Ellipse(const Vector2& position, const Vector2& dimensions)
	{
		Vector3 pos = { position, s_Data.Z += std::numeric_limits<float>::epsilon() };
		Vector3 dims = { dimensions, 0.0f };
		Ellipse(pos, dims);
	}

	void Ellipse(float x, float y, float w, float h)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Ellipse({ x, y, z }, { w, h, 0.0f });
	}

	void Line(const Vector3& p1, const Vector3& p2)
	{
		Vector3 pos = (p1 + p2) / 2.0f;
		float distance = glm::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
		Vector3 dims = Vector3(distance, s_Data.StrokeWeight, 0);
		float angle = glm::atan((float)(p2.y - p1.y) / (p2.x - p1.x)); // TODO: fix for 3D use quat ?

		pos = StandardisePosition(pos, dims);
		Vector3 c1 = StandardisePosition(p1, { 2, 2 });
		Vector3 c2 = StandardisePosition(p2, { 2, 2 });

		glm::vec4 strokeColour = glm::vec4(s_Data.StrokeColour.x, s_Data.StrokeColour.y, s_Data.StrokeColour.z, s_Data.StrokeWeight ? s_Data.StrokeColour.w : 0);
		if (!s_Data.StrokeWeight)
			strokeColour = glm::vec4(0);

		Renderer::Rect(pos, dims, glm::vec3(0.0f, 0.0f, angle), strokeColour, glm::vec4(0), 0.0f, nullptr, 1.0f);

		float diameter = s_Data.StrokeWeight;
		glm::vec3 circleDims(diameter, diameter, 0);

		Renderer::Ellipse(c1, circleDims, glm::vec3(0.0f), strokeColour, glm::vec4(0), 0.0f);
		Renderer::Ellipse(c2, circleDims, glm::vec3(0.0f), strokeColour, glm::vec4(0), 0.0f);
	}

	void Line(const Vector2& p1, const Vector2& p2)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Line({ p1, z }, { p2, z });
	}

	void Line(float x1, float y1, float x2, float y2)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Line({ x1, y1, z }, { x2, y2, z });
	}

	void BezierCurve(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4)
	{
		std::vector<Vector3> points;
		for (float t = 0.0f; t <= 1.0001f; t += 0.02f)
		{
			Vector3 b = p1 * (float)glm::pow(1 - t, 3) + p2 * 3.0f * (float)glm::pow(1 - t, 2) * t + p3 * 3.0f * (1 - t) * (float)glm::pow(t, 2) + p4 * (float)glm::pow(t, 3);
			points.push_back(b);
		}

		for (int i = 1; i < points.size(); i++)
		{
			Vector3& v1 = points[i - 1];
			Vector3& v2 = points[i];
			Line(v1, v2);
		}
	}

	void BezierCurve(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Vector2& p4)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		BezierCurve({ p1, z }, { p2, z }, { p3, z }, { p4, z });
	}

	void BezierCurve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		BezierCurve({ x1, y1, z }, { x2, y2, z }, { x3, y3, z }, { x4, y4, z });
	}

	Font LoadFont(const char* filepath)
	{
		::Font* font = new ::Font(filepath);
		s_Data.FontsMap[font->FontAtlas->RendererID()] = font;
		return font->FontAtlas->RendererID();
	}

	void FontSize(float size)
	{
		s_Data.FontSize = size;
	}

	void Text(const char* text, const Vector3& position, Font font)
	{
		Vector3 dims = GetTextDimensions(text, s_Data.FontsMap[font]) * s_Data.FontSize;
		Vector3 pos = position - Vector3(dims.x / 2.0f, dims.y / 4.0f, 0.0f);
		pos = StandardisePosition(pos, dims);
		Renderer::Text(pos, s_Data.Rotation, s_Data.StrokeColour, text, s_Data.FontsMap[font], s_Data.FontSize);
	}

	void Text(const char* text, const Vector2& position, Font font)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Vector3 pos = { position, z };
		Text(text, pos, font);
	}

	void Text(const char* text, float x, float y, Font font)
	{
		float z = s_Data.Z += std::numeric_limits<float>::epsilon();
		Vector3 position = { x, y, z };
		Text(text, position, font);
	}

	void Point(int x, int y)
	{
		Vector4 oldFill = s_Data.FillColour;
		s_Data.FillColour = s_Data.StrokeColour;
		Rect(x, y, 2, 2);
		s_Data.FillColour = oldFill;
	}

	Texture CreateTexture(const char* filepath)
	{
		Texture2D* t = new Texture2D(filepath);
		s_Data.TexturesMap[t->RendererID()] = t;
		return t->RendererID();
	}

	void Fill(const Vector4& colour)
	{
		s_Data.FillColour = colour / 255.0f;
	}

	void Stroke(const Vector4& colour)
	{
		s_Data.StrokeColour = colour / 255.0f;
	}

	void StrokeWeight(float weight)
	{
		s_Data.StrokeWeight = weight;
	}

	void NoFill()
	{
		s_Data.FillColour = Vector4(0.0f);
	}

	void NoStroke()
	{
		s_Data.StrokeWeight = 0;
	}

	void Rotate(float angle, const Vector3& axes)
	{
		if (axes.x == 1)
			s_Data.Rotation.x = angle;
		if (axes.y == 1)
			s_Data.Rotation.y = angle;
		if (axes.z == 1)
			s_Data.Rotation.z = angle;
	}

	void NoRotate()
	{
		s_Data.Rotation = Vector3(0.0f);
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

	Sound LoadSound(const char* filepath, bool looping)
	{
		static uint32_t id = 0;
		::Sound* sound = new ::Sound(filepath, looping);
		s_Data.SoundsMap[id] = sound;
		return id++;
	}

	void PlaySound(Sound sound)
	{
		s_Data.SoundsMap[sound]->Play();
	}

	void StopSound(Sound sound)
	{
		s_Data.SoundsMap[sound]->Stop();
	}

	bool IsSoundPlaying(Sound sound)
	{
		return s_Data.SoundsMap[sound]->IsPlaying();
	}

	void SetSoundVolume(Sound sound, float volume)
	{
		volume = std::clamp(volume, 0.0f, 1.0f);
		s_Data.SoundsMap[sound]->SetVolume(volume);
	}

	float GetSoundVolume(Sound sound)
	{
		return s_Data.SoundsMap[sound]->Volume();
	}

	Vector2 MousePosition()
	{
		double xpos, ypos;
		GLFWwindow* window = Application::Get().GetWindow()->NativeWindow();
		glfwGetCursorPos(window, &xpos, &ypos);
		glm::vec4 pos = ::Camera::Get().GetWorldPosition(xpos, ypos);

		if (s_Data.OriginPosition == Canvas::OriginPosition::Center)
		{
			pos.x -= WindowWidth() / 2;
			pos.y -= WindowHeight() / 2;
		}
		else if (s_Data.OriginPosition == OriginPosition::BottomRight)
			pos.x = WindowWidth() - pos.x;
		else if (s_Data.OriginPosition == OriginPosition::TopLeft)
			pos.y = WindowHeight() - pos.y;
		else if (s_Data.OriginPosition == OriginPosition::TopRight)
		{
			pos.x = WindowWidth() - pos.x;
			pos.y = WindowHeight() - pos.y;
		}

		return Vector2(pos.x, pos.y);
	}

	uint32_t WindowWidth()
	{
		return ::Camera::Get().WindowWidth();
	}

	uint32_t WindowHeight()
	{
		return ::Camera::Get().WindowHeight();
	}

	float FontLineHeight(Font font)
	{
		return s_Data.FontSize * s_Data.FontsMap[font]->FontGeometry.getMetrics().lineHeight;
	}

	void Shutdown()
	{
		for (auto texture : s_Data.TexturesMap)
			delete texture.second;
		for (auto font : s_Data.FontsMap)
			delete font.second;
		for (auto sound : s_Data.SoundsMap)
			delete sound.second;
	}
}
