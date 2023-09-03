#pragma once
#include "Core.h"

#include <cstdint>
#include <functional>

class Window;
class Camera;

class CANVAS_API Application
{
public:
	~Application();

	static Application& Get();
	void Run(std::function<void()> drawFun);

	float FPS();
	Window* GetWindow() { return m_Window; };

	Application(const Application&) = delete;
private:
	Application(const char* name, uint32_t windowWidth, uint32_t windowHeight, bool vsync);
private:
	Window* m_Window = nullptr;
	Camera& m_Camera;
};
