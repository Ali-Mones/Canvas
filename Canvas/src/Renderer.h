#pragma once

#include "Core.h"

#include <cstdint>

class Shader;

class API Renderer
{
public:
	static void Init();
	static void Shutdown();

	static void StartBatch();
	static void Flush();

	static void Clear(uint32_t r, uint32_t g, uint32_t b);

	static void Rect(int x, int y, int w, int h = -1);//, int tl = 0, int tr = 0, int bl = 0, int br = 0);
	static void Ellipse(int x, int y, int w, int h = -1);
	static void Quad(int p1, int p2, int p3, int p4);
	static void Line(int x1, int y1, int x2, int y2);

	static uint32_t QuadCount();
	static uint32_t DrawCalls();

	static Shader* RectShader();
	static Shader* CircleShader();
	static Shader* LineShader();

	static void Fill(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	static void Fill(uint32_t grey) { Fill(grey, grey, grey, 255); }

	static void Stroke(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
	static void Stroke(uint32_t grey) { Stroke(grey, grey, grey, 255); }

	static void StrokeWeight(uint32_t weight);
};
