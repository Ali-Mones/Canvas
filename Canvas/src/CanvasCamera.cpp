#include "CanvasCamera.h"
#include "Camera.h"
#include "Canvas.h"

#include <iostream>

static float s_MotionSpeed = 100.0f;
static float s_ZoomSpeed = 100.0f;

void Canvas::Camera::MoveRight()
{
	::Camera::Get().MoveRight(s_MotionSpeed * 1 / Application::Get().FPS());
}

void Canvas::Camera::MoveLeft()
{
	::Camera::Get().MoveLeft(s_MotionSpeed * 1 / Application::Get().FPS());
}

void Canvas::Camera::MoveUp()
{
	::Camera::Get().MoveUp(s_MotionSpeed * 1 / Application::Get().FPS());
}

void Canvas::Camera::MoveDown()
{
	::Camera::Get().MoveDown(s_MotionSpeed * 1 / Application::Get().FPS());
}

void Canvas::Camera::ZoomIn()
{
	::Camera::Get().ZoomIn(s_MotionSpeed * 1 / Application::Get().FPS());
}

void Canvas::Camera::ZoomOut()
{
	::Camera::Get().ZoomOut(s_MotionSpeed * 1 / Application::Get().FPS());
}

void Canvas::Camera::MotionSpeed(float speed)
{
	s_MotionSpeed = speed;
}

float Canvas::Camera::MotionSpeed()
{
	return s_MotionSpeed;
}

void Canvas::Camera::ZoomSpeed(float speed)
{
	s_ZoomSpeed = speed;
}

float Canvas::Camera::ZoomSpeed()
{
	return s_ZoomSpeed;
}
