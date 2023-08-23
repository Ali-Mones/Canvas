#pragma once

#include "Application.h"

extern void Setup();
extern void Draw();

int main()
{
	Setup();
	Application& app = Application::Get();
	app.Run(Draw);
}
