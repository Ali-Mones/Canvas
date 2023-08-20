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
	virtual ~Application();

	Application(const Application&) = delete;
private:
	Application(const char* name, uint32_t windowWidth, uint32_t windowHeight, bool vsync);
	Window* m_Window = nullptr;
	Camera& m_Camera;
};

namespace Canvas {
	void API CreateCanvas(uint32_t width, uint32_t height);
}
