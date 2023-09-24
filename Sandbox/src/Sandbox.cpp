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

Sound sound;

void Input();

void Setup()
{
	/*
	* TODO: checkout precompiled header
	* TODO: Check sorting for transparency on batching
	* TODO: Return origin to be center
	*/
	CreateCanvas(1280, 720);
	t = CreateTexture("res/textures/heart.png");
	player = CreateTexture("res/textures/player.png");
	arial = LoadFont("C:/Windows/Fonts/Arial.ttf");
	cambria = LoadFont("C:/Windows/Fonts/cambriab.ttf");
	sound = LoadSound("res/sounds/chomp.wav", true);
	SetSoundVolume(sound, 0.2f);
}

void Draw()
{
	SetOriginPosition(OriginPosition::Center);
	SetPositionMode(PositionMode::Center);
	Input();
	Clear(0, 255, 0);

	Rect(0, 0, 200, 200);

	Stroke(24);
	FontSize(120);
	Text("today we kill your face", 0, 0, arial);
}

void Input()
{
	Input::SetKeyCallback(Key::H, Action::Press, []() { FlipTextureHorizontally(hflip = !hflip); });
	Input::SetKeyCallback(Key::V, Action::Press, []() { FlipTextureVertically(vflip = !vflip); });

	Canvas::Camera::MotionSpeed(200);
	Canvas::Camera::ZoomSpeed(200);

	if (Input::IsKeyPressed(Key::Space))
		Canvas::Ellipse(Canvas::MouseX(), Canvas::MouseY(), 100, 100);

	if (Input::IsKeyPressed(Key::Right))
		Canvas::Camera::MoveRight();

	if (Input::IsKeyPressed(Key::Left))
		Canvas::Camera::MoveLeft();

	if (Input::IsKeyPressed(Key::Up))
		Canvas::Camera::MoveUp();

	if (Input::IsKeyPressed(Key::Down))
		Canvas::Camera::MoveDown();

	if (Input::IsKeyPressed(Key::Q))
		Canvas::Camera::ZoomIn();

	if (Input::IsKeyPressed(Key::E))
		Canvas::Camera::ZoomOut();
}
