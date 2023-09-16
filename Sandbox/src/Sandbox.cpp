#include "Canvas.h"
#include "EntryPoint.h"

#include <iostream>

using namespace Canvas;

Texture t;
Texture player;
Font arial;
Font cambria;
bool hflip = false;
bool vflip = false;

void Input();

void Setup()
{
	CreateCanvas(1280, 720);
	t = CreateTexture("res/textures/heart.png");
	player = CreateTexture("res/textures/player.png");
	arial = LoadFont("C:/Windows/Fonts/Arial.ttf");
	cambria = LoadFont("C:/Windows/Fonts/cambriab.ttf");
}

void Draw()
{
	/*	TODO:
			add custom vector classes + adding API changes for them
	*/

	SetPositionMode(PositionMode::Center);
	SetOriginPosition(OriginPosition::Center);
	Input();
	Clear(0, 0, 0);
	Stroke(255);
	FontSize(24);
	Text("today we kill your face", 0, 0, arial);
	//Text("hello my friends", 0, 400, cambria);

	Fill(255);
	Stroke(255, 0, 0);
	StrokeWeight(20);
	Ellipse(200, 200, 200, 200);
}

void Input()
{
	Input::SetKeyCallback(Key::H, Action::Press, []() { FlipTextureHorizontally(hflip = !hflip); });
	Input::SetKeyCallback(Key::V, Action::Press, []() { FlipTextureVertically(vflip = !vflip); });

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
