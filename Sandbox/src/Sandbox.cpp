#include "Canvas.h"
#include "EntryPoint.h"

using namespace Canvas;

CanvasTexture t;

void Setup()
{
	CreateCanvas(1280, 720);
	t = CreateTexture("res/textures/heart.png");
}

void Draw()
{
	/*	TODO:
			add comments to api,
			add camera wrapper class,
			check for performance using performance profiling tools,
			check making circles with bezier curves,
			text rendering,
	*/
	Clear(255, 255, 255);
	Fill(255);
	TexturedRect(200, 200, 90, 90, t);
}
