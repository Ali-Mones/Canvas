#include "Input.h"
#include "Application.h"
#include "Window.h"

#include <glfw3.h>

bool Input::IsKeyDown(KeyCode key)
{
	return glfwGetKey(Application::Get().GetWindow()->NativeWindow(), key) == GLFW_PRESS;
}

bool Input::IsKeyUp(KeyCode key)
{
	return glfwGetKey(Application::Get().GetWindow()->NativeWindow(), key) == GLFW_RELEASE;
}
