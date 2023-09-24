#pragma once

#include "Application.h"
#include "Input.h"
#include "CanvasCamera.h"

namespace Canvas {

	typedef std::uint32_t Texture;
	typedef std::uint32_t Font;
	typedef std::uint32_t Sound;

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

		template<typename T>
		operator T() { return *(T*)(this); }

		Vector2 operator +(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
		Vector2 operator +(float c) const { return Vector2(x + c, y + c); }
		void operator +=(const Vector2& other) { *this = *this + other; }
		void operator +=(float c) { *this = *this + c; }

		Vector2 operator -(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
		Vector2 operator -(float c) const { return Vector2(x - c, y - c); }
		void operator -=(const Vector2& other) { *this = *this - other; }
		void operator -=(float c) { *this = *this - c; }

		Vector2 operator *(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
		Vector2 operator *(float c) const { return Vector2(x * c, y * c); }
		void operator *=(const Vector2& other) { *this = *this * other; }
		void operator *=(float c) { *this = *this * c; }

		Vector2 operator /(float c) const { return Vector2(x / c, y / c); }
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

		Vector2 XY() { return Vector2(x, y); }
		Vector2 XZ() { return Vector2(x, z); }
		Vector2 YZ() { return Vector2(y, z); }

		template<typename T>
		operator T() { return *(T*)(this); }

		Vector3 operator +(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3 operator +(float c) const { return Vector3(x + c, y + c, z + c); }
		void operator +=(const Vector3& other) { *this = *this + other; }
		void operator +=(float c) { *this = *this + c; }

		Vector3 operator -(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3 operator -(float c) const { return Vector3(x - c, y - c, z - c); }
		void operator -=(const Vector3& other) { *this = *this - other; }

		Vector3 operator *(const Vector3& other) const { return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3 operator *(float c) const { return Vector3(x * c, y * c, z * c); }
		void operator *=(const Vector3& other) { *this = *this * other; }
		void operator *=(float c) { *this = *this * c; }

		Vector3 operator /(float c) const { return Vector3(x / c, y / c, z / c); }
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
		
		Vector2 XY() { return Vector2(x, y); }
		Vector2 XZ() { return Vector2(x, z); }
		Vector2 XW() { return Vector2(x, w); }
		Vector2 YZ() { return Vector2(y, z); }
		Vector2 YW() { return Vector2(y, w); }
		Vector2 ZW() { return Vector2(z, w); }
		Vector3 XYZ() { return Vector3(x, y, z); }
		Vector3 XYW() { return Vector3(x, y, w); }
		Vector3 XZW() { return Vector3(x, z, w); }
		Vector3 YZW() { return Vector3(y, z, w); }

		template<typename T>
		operator T() { return *(T*)(this); }

		Vector4 operator +(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4 operator +(float c) const { return Vector4(x + c, y + c, z + c, w + c); }
		void operator +=(const Vector4& other) { *this = *this + other; }
		void operator +=(float c) { *this = *this + c; }

		Vector4 operator -(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4 operator -(float c) const { return Vector4(x - c, y - c, z - c, w - c); }
		void operator -=(const Vector4& other) { *this = *this - other; }
		void operator -=(float c) { *this = *this - c; }

		Vector4 operator *(const Vector4& other) const { return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4 operator *(float c) const { return Vector4(x * c, y * c, z * c, w * c); }
		void operator *=(const Vector4& other) { *this = *this * other; }
		void operator *=(float c) { *this = *this * c; }

		Vector4 operator /(float c) const { return Vector4(x / c, y / c, z / c, w / c); }
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
	void CANVAS_API Clear(const Vector3& colour); 
	inline void CANVAS_API Clear(float r, float g, float b) { Clear({ r, g, b }); }
	inline void CANVAS_API Clear(float grey) { Clear({ grey, grey, grey }); }

	/**
	 * @brief Draw a rectangle
	 */
	void CANVAS_API Rect(const Vector3& position, const Vector3& dimensions);
	void CANVAS_API Rect(const Vector2& position, const Vector2& dimensions);
	void CANVAS_API Rect(float x, float y, float w, float h);

	/**
	 * @brief Draw a textured rectangle
	 */
	void CANVAS_API TexturedRect(const Vector3& position, const Vector3& dimensions, Texture texture);
	void CANVAS_API TexturedRect(const Vector2& position, const Vector2& dimensions, Texture texture);
	void CANVAS_API TexturedRect(float x, float y, float w, float h, Texture texture);

	/**
	 * @brief Draw an ellipse
	 */
	void CANVAS_API Ellipse(const Vector3& position, const Vector3& dimensions);
	void CANVAS_API Ellipse(const Vector2& position, const Vector2& dimensions);
	void CANVAS_API Ellipse(float x, float y, float w, float h);

	/**
	 * @brief Draw a line from point (x1, y1) to point (x2, y2)
	 */
	void CANVAS_API Line(const Vector3& p1, const Vector3& p2);
	void CANVAS_API Line(const Vector2& p1, const Vector2& p2);
	void CANVAS_API Line(float x1, float y1, float x2, float y2);

	/**
	 * @brief Draw a bezier curve from point (x1, y1) to point (x4, y4) using 2 control points (x2, y2), (x3, y3)
	 */
	void CANVAS_API BezierCurve(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
	void CANVAS_API BezierCurve(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Vector2& p4);
	void CANVAS_API BezierCurve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

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
	void CANVAS_API FontSize(float size);

	/**
	 * @return Line height for the set font.
	 */
	float CANVAS_API FontLineHeight(Font font);

	/**
	 * @brief Draw text. Text colour is affected by stroke colour.
	 */
	void CANVAS_API Text(const char* text, const Vector3& position, Font font);
	void CANVAS_API Text(const char* text, const Vector2& position, Font font);
	void CANVAS_API Text(const char* text, float x, float y, Font font);

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
	void CANVAS_API Fill(const Vector4& colour);
	inline void CANVAS_API Fill(const Vector3& colour) { Fill({ colour, 255 }); }
	inline void CANVAS_API Fill(float r, float g, float b, float a = 255) { Fill({ r, g, b, a }); }
	inline void CANVAS_API Fill(float grey) { Fill({ grey, grey, grey, 255 }); }

	/**
	 * @brief Set the stroke colour of the next drawn shape
	 */
	void CANVAS_API Stroke(const Vector4& colour);
	inline void CANVAS_API Stroke(const Vector3& colour) { Stroke({ colour, 255 }); }
	inline void CANVAS_API Stroke(float r, float g, float b, float a = 255) { Stroke({ r, g, b, a }); }
	inline void CANVAS_API Stroke(float grey) { Stroke({ grey, grey, grey, 255 }); }

	/**
	 * @brief Set the stroke weight of the next drawn shapes
	 */
	void CANVAS_API StrokeWeight(float weight);

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

	/**
	 * @brief Loads a sound file.
	 * @brief WARNING: Do not use this funcion in the Draw() function but in the Setup() function.
	 * 
	 * @return ID of the sound.
	 */
	Sound CANVAS_API LoadSound(const char* filepath, bool looping);

	/**
	 * @brief Starts playing the sound.
	 */
	void CANVAS_API PlaySound(Sound sound);

	/**
	 * @brief Stops playing the sound.
	 */
	void CANVAS_API StopSound(Sound sound);

	/**
	 * @return true if sound is playing, false otherwise.
	 */
	bool CANVAS_API IsSoundPlaying(Sound sound);

	/**
	 * @brief Sets the volume of the sound. Values should be from 0 to 1.
	 */
	void CANVAS_API SetSoundVolume(Sound sound, float volume);
	
	/**
	 * @return The volume of the sound, a value from 0 to 1.
	 */
	float CANVAS_API GetSoundVolume(Sound sound);

	/*
	 * @return Current position of the mouse.
	 */
	Vector2 CANVAS_API MousePosition();

	/**
	 * @return x-coordinate of the mouse.
	 */
	inline float CANVAS_API MouseX() { return MousePosition().x; }

	/**
	 * @return y-coordinate of the mouse.
	 */
	inline float CANVAS_API MouseY() { return MousePosition().y; }

	/**
	 * @return Width of the window.
	 */
	uint32_t CANVAS_API WindowWidth();

	/**
	 * @return Height of the window.
	 */
	uint32_t CANVAS_API WindowHeight();
}
