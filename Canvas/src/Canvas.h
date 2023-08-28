#pragma once

#include "Application.h"
#include "Input.h"

namespace Canvas {
	void API CreateCanvas(uint32_t width, uint32_t height);
	void API Clear(uint32_t r, uint32_t g, uint32_t b);

	void API Rect(int x, int y, uint32_t w, uint32_t h);
	void API Ellipse(int x, int y, int w, int h = -1);
	void API Line(int x1, int y1, int x2, int y2);
	void API BezierCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

	void API Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	inline void API Fill(uint32_t grey) { Fill(grey, grey, grey, 255); }

	void API Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	inline void API Stroke(uint32_t grey) { Stroke(grey, grey, grey, 255); }
	void API StrokeWeight(uint32_t weight);

	void API NoFill();
	void API NoStroke();

	double API MouseX();
	double API MouseY();

	uint32_t API WindowWidth();
	uint32_t API WindowHeight();
}
