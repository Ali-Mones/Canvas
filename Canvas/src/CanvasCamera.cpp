#include "CanvasCamera.h"
#include "Camera.h"
#include "Canvas.h"

#include <iostream>

static float s_MotionSpeed = 100.0f;
static float s_ZoomSpeed = 100.0f;

void CanvasCamera::MoveRight()
{
	Camera::Get().MoveRight(s_MotionSpeed * 1 / Application::Get().FPS());
}

void CanvasCamera::MoveLeft()
{
	Camera::Get().MoveLeft(s_MotionSpeed * 1 / Application::Get().FPS());
}

void CanvasCamera::MoveUp()
{
	Camera::Get().MoveUp(s_MotionSpeed * 1 / Application::Get().FPS());
}

void CanvasCamera::MoveDown()
{
	Camera::Get().MoveDown(s_MotionSpeed * 1 / Application::Get().FPS());
}

void CanvasCamera::ZoomIn()
{
	Camera::Get().ZoomIn(s_MotionSpeed * 1 / Application::Get().FPS());
}

void CanvasCamera::ZoomOut()
{
	Camera::Get().ZoomOut(s_MotionSpeed * 1 / Application::Get().FPS());
}

void CanvasCamera::MotionSpeed(float speed)
{
	s_MotionSpeed = speed;
}

float CanvasCamera::MotionSpeed()
{
	return s_MotionSpeed;
}

void CanvasCamera::ZoomSpeed(float speed)
{
	s_ZoomSpeed = speed;
}

float CanvasCamera::ZoomSpeed()
{
	return s_ZoomSpeed;
}
