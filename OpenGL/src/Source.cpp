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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Help me pls!!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // vsync on
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed\n";
        return -1;
    }

    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

    std::vector<float> vertices =
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    std::vector<uint32_t> indices =
    { 
      0, 1, 2,
      2, 3, 0
    };

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.5f, 1.0f);

    //create a vertex array object
    VertexArray va;
    VertexBufferLayout layout;
    layout.Push<float>(2);

    /* create a vertex buffer and bind it to use it */
    VertexBuffer vb(vertices.data(), vertices.size() * sizeof(float));
    va.AddBuffer(vb, layout);

    // index buffer
    IndexBuffer ib(indices.data(), indices.size());

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        renderer.Draw(va, ib, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
};