#include "Canvas.h"
#include "EntryPoint.h"
#include <iostream>

using namespace Canvas;

void Setup()
{
	CreateCanvas(1280, 720);
}

void Draw()
{
	// TODO: Point(int x, int y)
	Clear(255, 255, 255);

	Fill(0);
	Stroke(100, 200, 0);
	StrokeWeight(5);
	NoFill();
	Rect(200, 200, 300, 100);

	Rect(300, 230, 300, 100);

	StrokeWeight(10);
	Line(1000, 100, 1100, 300);

	Fill(255, 0, 0);
	Stroke(0, 0, 255);
	StrokeWeight(20);
	Ellipse(400, 400, 500, 500);

	StrokeWeight(10);
	Line(100, 200, 900, 100);
	BezierCurve(0, 400, 400, 100, 800, 700, 1280, 400);
}
