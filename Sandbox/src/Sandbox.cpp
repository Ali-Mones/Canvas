#include "EntryPoint.h"

void Setup()
{
	Canvas::CreateCanvas(1280, 720);
}

void Draw()
{
	// TODO: move shaders to opengl project
	Renderer::Clear(255, 255, 255);

	Renderer::Fill(10, 200, 240);
	Renderer::Stroke(100);
	Renderer::StrokeWeight(1);
	Renderer::Rect(200, 200, 300);

	Renderer::Stroke(0);
	Renderer::StrokeWeight(10);
	Renderer::Line(0, 0, 200, 200);

	Renderer::Stroke(255, 0, 255);
	Renderer::Line(100, 200, 900, 100);
}
