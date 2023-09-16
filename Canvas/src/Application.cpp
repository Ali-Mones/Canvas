#include "Application.h"

#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Renderer.h"
#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "Canvas.h"

namespace Canvas {
    void Shutdown();
}

static uint32_t s_Width = -1;
static uint32_t s_Height = -1;

Application::Application(const char* name, uint32_t windowWidth, uint32_t windowHeight, bool vsync)
    : m_Camera(Camera::Init(windowWidth, windowHeight))
{
    m_Window = new Window(name, windowWidth, windowHeight, vsync);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(m_Window->NativeWindow(), true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImFont* font = io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/CascadiaCode.ttf", 18.0f);
    IM_ASSERT(font != nullptr);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    glfwSwapInterval(1); // enable vsync
    
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed\n";
        return;
    }

    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    Renderer::Init();
}

Application& Application::Get()
{
    assert(s_Width != -1 && s_Height != -1);
    static Application instance("Application", s_Width, s_Height, true);
    return instance;
}

void Application::Run(std::function<void()> drawFun)
{
    while (!glfwWindowShouldClose(m_Window->NativeWindow()))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Renderer::StartBatch();
        drawFun();
        Renderer::Flush();

        /* Window resizing */
        int width, height;
        glfwGetWindowSize(m_Window->NativeWindow(), &width, &height);
        m_Camera.OnWindowResize(width, height);

        glm::mat4 vp = m_Camera.ViewProjection();

        Renderer::RectShader()->SetUniformMat4f("u_ViewProjection", vp);
        Renderer::CircleShader()->SetUniformMat4f("u_ViewProjection", vp);
        Renderer::TextShader()->SetUniformMat4f("u_ViewProjection", vp);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window->NativeWindow());

        /* Poll for and process events */
        glfwPollEvents();
    }
}

Application::~Application()
{
    delete m_Window;
    Renderer::Shutdown();
    Canvas::Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

float Application::FPS()
{
    return ImGui::GetIO().Framerate;
}


namespace Canvas {
    void CreateCanvas(uint32_t width, uint32_t height)
    {
        s_Width = width;
        s_Height = height;
        Application::Get();
    }
}