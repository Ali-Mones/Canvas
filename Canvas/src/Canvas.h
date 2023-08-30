#pragma once

#include "Application.h"
#include "Input.h"
#include "Texture.h"

namespace Canvas {
	void CANVAS_API CreateCanvas(uint32_t width, uint32_t height);
	void CANVAS_API Clear(uint32_t r, uint32_t g, uint32_t b);

	void CANVAS_API Rect(int x, int y, uint32_t w, uint32_t h);
	void CANVAS_API TexturedRect(int x, int y, uint32_t w, uint32_t h, Texture* texture);
	void CANVAS_API Ellipse(int x, int y, int w, int h = -1);
	void CANVAS_API Line(int x1, int y1, int x2, int y2);
	void CANVAS_API BezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void CANVAS_API Point(int x, int y);

	void CANVAS_API Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	inline void CANVAS_API Fill(uint32_t grey) { Fill(grey, grey, grey, 255); }

	void CANVAS_API Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	inline void CANVAS_API Stroke(uint32_t grey) { Stroke(grey, grey, grey, 255); }
	void CANVAS_API StrokeWeight(uint32_t weight);

	void CANVAS_API NoFill();
	void CANVAS_API NoStroke();

	void CANVAS_API TextureTilingFactor(float tilingFactor);
	void CANVAS_API NoTextureTiling();

	double CANVAS_API MouseX();
	double CANVAS_API MouseY();

	uint32_t CANVAS_API WindowWidth();
	uint32_t CANVAS_API WindowHeight();
}
