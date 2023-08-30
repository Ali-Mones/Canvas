#include "Canvas.h"
#include "EntryPoint.h"

using namespace Canvas;

Texture* t;

void Setup()
{
	CreateCanvas(1280, 720);
	t = new Texture("res/textures/heart.png");
}

void Draw()
{
	// TODO: Point(int x, int y)
	Clear(255, 255, 255);

	Fill(0);
	Point(200, 200);
}
