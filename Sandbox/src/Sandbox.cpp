#include "Canvas.h"
#include "EntryPoint.h"

#include <iostream>

using namespace Canvas;

CanvasTexture t;
CanvasTexture player;
bool hflip = false;
bool vflip = false;

void Setup()
{
	CreateCanvas(1280, 720);
	t = CreateTexture("res/textures/heart.png");
	player = CreateTexture("res/textures/player.png");
}

void Draw()
{
	/*	TODO:
			check for performance using performance profiling tools,
			text rendering,
	*/

	Clear(0, 0, 0);
	Fill(255);

	Input::SetKeyCallback(Key::H, Action::Press, []() { FlipTextureHorizontally(hflip = !hflip); });
	Input::SetKeyCallback(Key::V, Action::Press, []() { FlipTextureVertically(vflip = !vflip); });

	TexturedRect(0, 0, 90, 90, player);

	Fill(0, 255, 0);
	Ellipse(200, 200, 90, 90);

	CanvasCamera::MotionSpeed(200);
	CanvasCamera::ZoomSpeed(200);

	if (Input::IsKeyPressed(Key::Space))
		Canvas::Ellipse(Canvas::MouseX(), Canvas::MouseY(), 100, 100);

	if (Input::IsKeyPressed(Key::Right))
		CanvasCamera::MoveRight();

	if (Input::IsKeyPressed(Key::Left))
		CanvasCamera::MoveLeft();

	if (Input::IsKeyPressed(Key::Up))
		CanvasCamera::MoveUp();

	if (Input::IsKeyPressed(Key::Down))
		CanvasCamera::MoveDown();

	if (Input::IsKeyPressed(Key::Q))
		CanvasCamera::ZoomIn();

	if (Input::IsKeyPressed(Key::E))
		CanvasCamera::ZoomOut();
}
