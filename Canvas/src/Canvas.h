#pragma once

#include "Application.h"
#include "Input.h"
#include "CanvasCamera.h"

typedef uint32_t Texture;
typedef uint32_t Font;

namespace Canvas {

	enum class PositionMode
	{
		Center,
		TopLeft,
		BottomLeft,
		TopRight,
		BottomRight
	};

	enum class OriginPosition
	{
		Center,
		TopLeft,
		BottomLeft,
		TopRight,
		BottomRight
	};

	struct Vector2
	{
	public:
		Vector2(float x)
			: x(x), y(x)
		{
		}

		Vector2(float x, float y)
			: x(x), y(y)
		{
		}

		Vector2 operator +(const Vector2& other) { return Vector2(x + other.x, y + other.y); }
		Vector2 operator +(float c) { return Vector2(x + c, y + c); }
		void operator +=(const Vector2& other) { *this = *this + other; }
		void operator +=(float c) { *this = *this + c; }

		Vector2 operator -(const Vector2& other) { return Vector2(x - other.x, y - other.y); }
		Vector2 operator -(float c) { return Vector2(x - c, y - c); }
		void operator -=(const Vector2& other) { *this = *this - other; }
		void operator -=(float c) { *this = *this - c; }

		Vector2 operator *(const Vector2& other) { return Vector2(x * other.x, y * other.y); }
		Vector2 operator *(float c) { return Vector2(x * c, y * c); }
		void operator *=(const Vector2& other) { *this = *this * other; }
		void operator *=(float c) { *this = *this * c; }

		Vector2 operator /(float c) { return Vector2(x / c, y / c); }
		void operator /=(float c) { *this = *this / c; }
	public:
		float x, y;
	};

	struct Vector3
	{
	public:
		Vector3(float x)
			: x(x), y(x), z(x)
		{
		}

		Vector3(Vector2 v2, float z)
			: x(v2.x), y(v2.y), z(z)
		{
		}

		Vector3(float x, float y, float z)
			: x(x), y(y), z(z)
		{
		}

		Vector3 operator +(const Vector3& other) { return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3 operator +(float c) { return Vector3(x + c, y + c, z + c); }
		void operator +=(const Vector3& other) { *this = *this + other; }
		void operator +=(float c) { *this = *this + c; }

		Vector3 operator -(const Vector3& other) { return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3 operator -(float c) { return Vector3(x - c, y - c, z - c); }
		void operator -=(const Vector3& other) { *this = *this - other; }
		void operator -=(float c) { *this = *this - c; }

		Vector3 operator *(const Vector3& other) { return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3 operator *(float c) { return Vector3(x * c, y * c, z * c); }
		void operator *=(const Vector3& other) { *this = *this * other; }
		void operator *=(float c) { *this = *this * c; }

		Vector3 operator /(float c) { return Vector3(x / c, y / c, z / c); }
		void operator /=(float c) { *this = *this / c; }
	public:
		float x, y, z;
	};

	struct Vector4
	{
	public:
		Vector4(float x)
			: x(x), y(x), z(x), w(x)
		{
		}

		Vector4(Vector3 v3, float w)
			: x(v3.x), y(v3.y), z(v3.z), w(w)
		{
		}

		Vector4(Vector2 v2, float z, float w)
			: x(v2.x), y(v2.y), z(z), w(w)
		{
		}

		Vector4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{
		}

		Vector4 operator +(const Vector4& other) { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4 operator +(float c) { return Vector4(x + c, y + c, z + c, w + c); }
		void operator +=(const Vector4& other) { *this = *this + other; }
		void operator +=(float c) { *this = *this + c; }

		Vector4 operator -(const Vector4& other) { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4 operator -(float c) { return Vector4(x - c, y - c, z - c, w - c); }
		void operator -=(const Vector4& other) { *this = *this - other; }
		void operator -=(float c) { *this = *this - c; }

		Vector4 operator *(const Vector4& other) { return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4 operator *(float c) { return Vector4(x * c, y * c, z * c, w * c); }
		void operator *=(const Vector4& other) { *this = *this * other; }
		void operator *=(float c) { *this = *this * c; }

		Vector4 operator /(float c) { return Vector4(x / c, y / c, z / c, w / c); }
		void operator /=(float c) { *this = *this / c; }
	public:
		float x, y, z, w;
	};

	/**
	 * @brief Create the canvas.
	 * @brief WARNING: This function must be called first thing in the Setup() function.
	 */
	void CANVAS_API CreateCanvas(uint32_t width, uint32_t height);

	/**
	 * @brief Sets the position of the origin.
	 */
	void CANVAS_API SetOriginPosition(OriginPosition position);

	/**
	 * @brief Sets the position of the coordinates relative to the drawn shapes.
	 */
	void CANVAS_API SetPositionMode(PositionMode mode);

	/**
	 * @brief Clear the whole canvas using the specified colour
	 */
	void CANVAS_API Clear(uint32_t r, uint32_t g, uint32_t b);
	inline void CANVAS_API Clear(uint32_t grey) { Clear(grey, grey, grey); }
	inline void CANVAS_API Clear(Vector3 colour) { Clear(colour.x, colour.y, colour.z); }

	/**
	 * @brief Draw a rectangle
	 */
	void CANVAS_API Rect(int x, int y, uint32_t w, uint32_t h);
	inline void CANVAS_API Rect(Vector2 position, Vector2 dimensions) { Rect(position.x, position.y, dimensions.x, dimensions.y); }

	/**
	 * @brief Draw a textured rectangle
	 */
	void CANVAS_API TexturedRect(int x, int y, uint32_t w, uint32_t h, Texture texture);
	inline void CANVAS_API TexturedRect(Vector2 position, Vector2 dimensions, Texture texture) { TexturedRect(position.x, position.y, dimensions.x, dimensions.y, texture); }

	/**
	 * @brief Draw an ellipse
	 */
	void CANVAS_API Ellipse(int x, int y, uint32_t w, uint32_t h);
	inline void CANVAS_API Ellipse(Vector2 position, Vector2 dimensions) { Ellipse(position.x, position.y, dimensions.x, dimensions.y); }

	/**
	 * @brief Draw a line from point (x1, y1) to point (x2, y2)
	 */
	void CANVAS_API Line(int x1, int y1, int x2, int y2);
	inline void CANVAS_API Line(Vector2 p1, Vector2 p2) { Line(p1.x, p1.y, p2.x, p2.y); }

	/**
	 * @brief Draw a bezier curve from point (x1, y1) to point (x4, y4) using 2 control points (x2, y2), (x3, y3)
	 */
	void CANVAS_API BezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	inline void CANVAS_API BezierCurve(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) { BezierCurve(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y); }

	/**
	 * @brief Load a font.
	 * @brief WARNING: Do not use this funcion in the Draw() function but in the Setup() function.
	 * 
	 * @return ID of the font.
	 */
	Font CANVAS_API LoadFont(const char* filepath);

	/**
	 * @brief Sets the font size
	 */
	void CANVAS_API FontSize(uint32_t size);

	/**
	 * @return Line height for the set font.
	 */
	float CANVAS_API FontLineHeight(Font font);

	/**
	 * @brief Draw text. Text colour is affected by stroke colour.
	 */
	void CANVAS_API Text(const char* text, int x, int y, Font font);
	inline void CANVAS_API Text(const char* text, Vector2 position, Font font) { Text(text, position.x, position.y, font); }

	/**
	 * @brief Draw a point of size 2x2 pixels. Point colour is affected by stroke colour.
	 */
	void CANVAS_API Point(int x, int y);
	inline void CANVAS_API Point(Vector2 position) { Point(position.x, position.y); }

	/**
	 * @brief Create a texture.
	 * @brief WARNING: Do not use this funcion in the Draw() function but in the Setup() function.
	 * 
	 * @return ID of the texture.
	 */
	Texture CANVAS_API CreateTexture(const char* filepath);

	/**
	 * @brief Set the fill colour of the next drawn shapes
	 */
	void CANVAS_API Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	inline void CANVAS_API Fill(uint32_t grey) { Fill(grey, grey, grey); }
	inline void CANVAS_API Fill(Vector3 colour) { Fill(colour.x, colour.y, colour.z); }
	inline void CANVAS_API Fill(Vector4 colour) { Fill(colour.x, colour.y, colour.z, colour.w); }

	/**
	 * @brief Set the stroke colour of the next drawn shape
	 */
	void CANVAS_API Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	inline void CANVAS_API Stroke(uint32_t grey) { Stroke(grey, grey, grey); }
	inline void CANVAS_API Stroke(Vector3 colour) { Stroke(colour.x, colour.y, colour.z); }
	inline void CANVAS_API Stroke(Vector4 colour) { Stroke(colour.x, colour.y, colour.z, colour.w); }

	/**
	 * @brief Set the stroke weight of the next drawn shapes
	 */
	void CANVAS_API StrokeWeight(uint32_t weight);

	/**
	 * @brief Disables filling of the next drawn shapes.
	 * @brief WARNING: Using this funcion with NoStroke() will not draw anything.
	 */
	void CANVAS_API NoFill();

	/**
	 * @brief Disables the stroke for the next drawn shapes
	 * @brief WARNING: Using this funcion with NoFill() will not draw anything.
	 */
	void CANVAS_API NoStroke();

	/**
	 * @brief Sets the tiling factor of the next drawn textures.
	 * @brief Drawn textured rectangles will be tilingFactor * tilingFactor of the texture.
	 */
	void CANVAS_API TextureTilingFactor(float tilingFactor);

	/**
	 * @brief Disables tiling of the next drawn textures textures.
	 */
	void CANVAS_API NoTextureTiling();

	/**
	 * @brief Set flipping of the next drawn textures about the y-axis
	 */
	void CANVAS_API FlipTextureHorizontally(bool value);

	/**
	 * @brief Set flipping of the next drawn textures about the x-axis
	 */
	void CANVAS_API FlipTextureVertically(bool value);

	/*
	 * @return Current position of the mouse.
	 */
	Vector2 CANVAS_API MousePosition();

	/**
	 * @return x-coordinate of the mouse.
	 */
	float CANVAS_API MouseX();

	/**
	 * @return y-coordinate of the mouse.
	 */
	float CANVAS_API MouseY();

	/**
	 * @return Width of the window.
	 */
	uint32_t CANVAS_API WindowWidth();

	/**
	 * @return Height of the window.
	 */
	uint32_t CANVAS_API WindowHeight();
}
