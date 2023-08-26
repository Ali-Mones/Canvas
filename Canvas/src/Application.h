#pragma once
#include "Core.h"

#include <cstdint>
#include <functional>

class Window;
class Camera;

class API Application
{
public:
	static Application& Get();
	void Run(std::function<void()> drawFun);
	Window* GetWindow() { return m_Window; };
	~Application();

	Application(const Application&) = delete;
private:
	Application(const char* name, uint32_t windowWidth, uint32_t windowHeight, bool vsync);
	Window* m_Window = nullptr;
	Camera& m_Camera;
};
