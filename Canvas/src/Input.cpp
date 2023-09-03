#include <glfw3.h>
#include <iostream>
#include <map>

#include "Input.h"
#include "Application.h"
#include "Window.h"

std::map<std::pair<int, int>, std::function<void()>> s_Map;

bool Input::IsKeyPressed(Key key)
{
	return glfwGetKey(Application::Get().GetWindow()->NativeWindow(), (int)key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
	return glfwGetMouseButton(Application::Get().GetWindow()->NativeWindow(), (int)button) == GLFW_PRESS;
}

bool Input::IsKeyReleased(Key key)
{
	return glfwGetKey(Application::Get().GetWindow()->NativeWindow(),(int)key) == GLFW_RELEASE;
}

bool Input::IsMouseButtonReleased(MouseButton button)
{
	return glfwGetMouseButton(Application::Get().GetWindow()->NativeWindow(), (int) button) == GLFW_RELEASE;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (s_Map.count({ key, action }))
		s_Map[{ key, action }]();
}

void Input::SetKeyCallback(Key key, Action action, std::function<void()> func)
{
	s_Map[{ (int)key, (int)action }] = func;
	glfwSetKeyCallback(Application::Get().GetWindow()->NativeWindow(), KeyCallback);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (s_Map.count({ button, action }))
		s_Map[{ button, action }]();
}

void Input::SetMouseButtonCallback(MouseButton button, Action action, std::function<void()> func)
{
	s_Map[{ (int)button, (int)action }] = func;
	glfwSetMouseButtonCallback(Application::Get().GetWindow()->NativeWindow(), MouseButtonCallback);
}
