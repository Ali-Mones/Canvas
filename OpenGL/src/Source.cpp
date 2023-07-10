#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include "Vertex.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define ASSERT(x) if (!(x)) __debugbreak();

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Help me pls!!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // vsync on
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //io.Fonts->AddFontDefault();
    ImFont* font = io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/CascadiaCode.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(font != nullptr);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed\n";
        return -1;
    }

    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    

    glm::mat4 proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    Shader shader("res/shaders/basic.shader");
    shader.Bind();

    Vertex v1 =
    {
        { -0.5, -0.5, 0, 1 },
        { 1, 1, 0, 1 },
        { 0, 0 },
        1
    };

    Vertex v2 =
    {
        { 0.5, -0.5, 0, 1 },
        { 1, 1, 0, 1 },
        { 1, 0 },
        1
    };

    Vertex v3 =
    {
        { 0.5, 0.5, 0, 1 },
        { 1, 1, 0, 1 },
        { 1, 1 },
        1
    };

    Vertex v4 =
    {
        { -0.5, 0.5, 0, 1 },
        { 1, 1, 0, 1 },
        { 0, 1 },
        1
    };

    std::vector<Vertex> vs({ v1, v2, v3, v4 });

    /* create a vertex buffer and bind it to use it */
    VertexBuffer vb(vs.data(), vs.size() * sizeof(Vertex));
    VertexArray va;
    va.AddBuffer(vb);

    // index buffer
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
    IndexBuffer ib(indices.data(), indices.size());

    Texture texture("res/textures/heart.png");
    texture.Bind();

    Renderer renderer(shader);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //VertexArray va;
    //VertexBuffer vb(1000 * 6);

    glm::vec2 q1Translate(-0.2, 0.1);
    glm::vec2 q2Translate(0.5, 0.1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Hello, world!");

            ImGui::DragFloat2("TranslateA", &q1Translate.x, 0.1, -1, 1);
            ImGui::DragFloat2("TranslateB", &q2Translate.x, 0.1, -1, 1);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::End();
        }

        /* Render here */
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        renderer.Clear();

        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(q1Translate, 0));
        //glm::mat4 mvp = proj * view * model;

        /*std::vector<Vertex> vertices;
        auto q1 = renderer.Rect(q2Translate, { 0.3, 0.4 }, { 1, 1, 0, 1 });
        auto q2 = renderer.Rect(q2Translate, { 0.3, 0.4 }, { 1, 1, 0, 1 });

        vertices.insert(vertices.end(), q1.begin(), q1.end());
        vertices.insert(vertices.end(), q2.begin(), q2.end());*/

        //vb.SetBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

        //renderer.StartBatch();

        //renderer.Quad(q1Translate, { 0.3, 0.4 }, { 1, 1, 1, 1 });
        //renderer.Quad(q2Translate, { 0.3, 0.4 }, { 1, 1, 0, 1 });

        //renderer.EndBatch();
        //renderer.Flush();

        renderer.Draw(va, ib, shader);

        //shader.SetUniformMat4f("u_ModelViewProjection", mvp);

        //model = glm::translate(glm::mat4(1.0f), glm::vec3(q2Translate, 0));
        //mvp = proj * view * model;
        //shader.SetUniformMat4f("u_ModelViewProjection", mvp);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
};