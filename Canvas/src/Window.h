#pragma once

#include <glfw3.h>

class Window
{
public:
	Window(const char* name, uint32_t windowWidth, uint32_t windowHeight, bool vsync);
	~Window();

	bool KeyboardInput(int key);
	bool MouseInput(int button);

	GLFWwindow* NativeWindow() { return m_NativeWindow; }
private:
	GLFWwindow* m_NativeWindow = nullptr;
};
