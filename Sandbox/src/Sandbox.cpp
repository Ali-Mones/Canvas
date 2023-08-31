#include "Canvas.h"
#include "EntryPoint.h"

using namespace Canvas;

CanvasTexture* t;

void Setup()
{
	CreateCanvas(1280, 720);
	t = new CanvasTexture("res/textures/heart.png");
}

void Draw()
{
	/*	TODO:
			add comments to api,
			add camera wrapper class,
			check for performance using performance profiling tools,
			check making circles with bezier curves,
			text rendering,
			finalise texture api to not depend on new keyword 
	*/
	Clear(255, 255, 255);

	Stroke(0, 0, 0);
	Fill(255, 0, 0);
	StrokeWeight(50);
	NoFill();
	Ellipse(400, 400, 500, 200);
}
