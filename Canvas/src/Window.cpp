#include "Window.h"
#include <glfw3.h>

Window::Window(const char* name, uint32_t windowWidth, uint32_t windowHeight, bool vsync)
{
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    m_NativeWindow = glfwCreateWindow(windowWidth, windowHeight, name, NULL, NULL);
    if (!m_NativeWindow)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_NativeWindow);

    glfwSwapInterval(vsync ? 1 : 0);
}

Window::~Window()
{
    delete m_NativeWindow;
}

bool Window::KeyboardInput(int key)
{
    return glfwGetKey(m_NativeWindow, key) == GLFW_PRESS;
}

bool Window::MouseInput(int button)
{
    return glfwGetMouseButton(m_NativeWindow, button) == GLFW_PRESS;
}