#pragma once

#include <cstdint>
#include "Core.h"

class CANVAS_API CanvasCamera
{
public:
	static void MoveRight();
	static void MoveLeft();
	static void MoveUp();
	static void MoveDown();

	static void ZoomIn();
	static void ZoomOut();

	static float MotionSpeed();
	static void MotionSpeed(float speed);

	static float ZoomSpeed();
	static void ZoomSpeed(float speed);
};
