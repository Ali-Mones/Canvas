#pragma once

#include "Application.h"
#include "Input.h"
#include "CanvasCamera.h"

typedef uint32_t CanvasTexture;

namespace Canvas {
	
	/**
	 * @brief Create the canvas.
	 * @brief WARNING: This function must be called in the Setup() function.
	 */
	void CANVAS_API CreateCanvas(uint32_t width, uint32_t height);

	/**
	 * @brief Clear the whole canvas using the specified colour
	 */
	void CANVAS_API Clear(uint32_t r, uint32_t g, uint32_t b);

	/**
	 * @brief Draw a rectangle
	 */
	void CANVAS_API Rect(int x, int y, uint32_t w, uint32_t h);

	/**
	 * @brief Draw a textured rectangle
	 */
	void CANVAS_API TexturedRect(int x, int y, uint32_t w, uint32_t h, CanvasTexture texture);

	/**
	 * @brief Draw an ellipse
	 */
	void CANVAS_API Ellipse(int x, int y, uint32_t w, uint32_t h);

	/**
	 * @brief Draw a line from point (x1, y1) to point (x2, y2)
	 */
	void CANVAS_API Line(int x1, int y1, int x2, int y2);

	/**
	 * @brief Draw a bezier curve from point (x1, y1) to point (x4, y4) using 2 control points (x2, y2), (x3, y3)
	 */
	void CANVAS_API BezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

	/**
	 * @brief Sets the font size
	 */
	void CANVAS_API FontSize(uint32_t size);

	/**
	 * @brief Draw text. Text colour is affected by stroke colour.
	 */
	void CANVAS_API Text(const char* text, int x, int y);

	/**
	 * @brief Draw a point of size 2x2 pixels. Point colour is affected by stroke colour.
	 */
	void CANVAS_API Point(int x, int y);

	/**
	 * @brief Create a texture.
	 * @brief WARNING: Do not use this funcion in the Draw() function but in the Setup() function.
	 * 
	 * @return ID of the texture.
	 */
	CanvasTexture CANVAS_API CreateTexture(const char* filepath);

	/**
	 * @brief Set the fill colour of the next drawn shapes
	 */
	void CANVAS_API Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);

	/**
	 * @brief Set the fill colour of the next drawn shape
	 */
	inline void CANVAS_API Fill(uint32_t grey) { Fill(grey, grey, grey, 255); }

	/**
	 * @brief Set the stroke colour of the next drawn shape
	 */
	void CANVAS_API Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);

	/**
	 * @brief Set the stroke colour of the next drawn shape
	 */
	inline void CANVAS_API Stroke(uint32_t grey) { Stroke(grey, grey, grey, 255); }

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

	/**
	 * @return x-coordinate of the mouse.
	 */
	double CANVAS_API MouseX();

	/**
	 * @return y-coordinate of the mouse.
	 */
	double CANVAS_API MouseY();

	/**
	 * @return Width of the window.
	 */
	uint32_t CANVAS_API WindowWidth();

	/**
	 * @return Height of the window.
	 */
	uint32_t CANVAS_API WindowHeight();
}
