#include "Canvas.h"
#include "EntryPoint.h"

#include <iostream>

CanvasTexture t;

void Setup()
{
	Canvas::CreateCanvas(1280, 720);
	t = Canvas::CreateTexture("res/textures/sadsa.png");
}

void Draw()
{
	/*	TODO:
			check for performance using performance profiling tools,
			check making circles with bezier curves,
			text rendering,
			handle errors for unfound texture file
	*/

	Canvas::Clear(255, 255, 255);
	Canvas::Fill(255);
	Canvas::TexturedRect(0, 0, 90, 90, t);
	Canvas::Fill(200);
	Canvas::Ellipse(200, 200, 100, 100);

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
