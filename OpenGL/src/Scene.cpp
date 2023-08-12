#include "Scene.h"
#include "Renderer.h"
#include <imgui/imgui.h>

Scene::Scene(GLFWwindow* window)
	: m_Window(window), m_Camera(1280, 720)
{
}

bool Scene::KeyboardInput(int key)
{
	return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Scene::MouseInput(int button)
{
	return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
}

void Scene::OnUpdate()
{
	OnInputUpdate();
	OnRender();
	OnImGuiRender();
}

void Scene::OnInputUpdate()
{
	if (KeyboardInput(GLFW_KEY_LEFT))
		m_Camera.MoveLeft();

	if (KeyboardInput(GLFW_KEY_RIGHT))
		m_Camera.MoveRight();

	if (KeyboardInput(GLFW_KEY_UP))
		m_Camera.MoveUp();

	if (KeyboardInput(GLFW_KEY_DOWN))
		m_Camera.MoveDown();

	if (KeyboardInput(GLFW_KEY_A))
		m_Rects[0].x -= 10;

	if (KeyboardInput(GLFW_KEY_D))
		m_Rects[0].x += 10;

	if (KeyboardInput(GLFW_KEY_S))
		m_Rects[0].y -= 10;

	if (KeyboardInput(GLFW_KEY_W))
		m_Rects[0].y += 10;

	if (KeyboardInput(GLFW_KEY_Q))
		m_Camera.ZoomOut();

	if (KeyboardInput(GLFW_KEY_E))
		m_Camera.ZoomIn();

	if (MouseInput(GLFW_MOUSE_BUTTON_LEFT))
	{
		double xpos, ypos;
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		glfwGetCursorPos(m_Window, &xpos, &ypos);

		glm::vec4 model = m_Camera.GetWorldPosition(xpos, ypos, width, height);
		SubmitRect(model.x, model.y, 100, 100, glm::vec4(255, 0, 0, 255));
	}
}

void Scene::OnRender()
{
	Renderer::Clear(m_ClearColour);
	Renderer::StartBatch();

	Renderer::Fill(100, 100, 100);
	Renderer::StrokeWeight(1);

	for (auto& rect : m_Rects)
	{
		Renderer::Fill(rect.Colour);
		Renderer::Rect(rect.x, rect.y, rect.w, rect.h);
	}

	for (auto& circle : m_Circles)
	{
		Renderer::Fill(circle.Colour);
		Renderer::Ellipse(circle.x, circle.y, circle.w, circle.h);
	}

	//for (auto& line : m_Lines)
		//Renderer::RenderLine(line);

	Renderer::Stroke(0);
	Renderer::StrokeWeight(10);
	Renderer::Line(0, 0, 200, 200);

	Renderer::Flush();
}

void Scene::OnImGuiRender()
{
	ImGui::Begin("Stats");

	auto& io = ImGui::GetIO();

	uint32_t drawCalls = Renderer::DrawCalls();
	uint32_t quadCount = Renderer::QuadCount();
	
	ImGui::Text("FPS = %f", io.Framerate);
	ImGui::Text("Draw Calls = %d, Quad Count = %d", drawCalls, quadCount);

	ImGui::End();
}

void Scene::SubmitRect(int x, int y, int w, int h, glm::vec4 colour)
{
	Rect rect = { x, y, w, h, colour };
	m_Rects.push_back(rect);
}

void Scene::SubmitCircle(int x, int y, int w, int h, glm::vec4 colour)
{
	Circle c = { x, y, w, h, colour };
	m_Circles.push_back(c);
}

void Scene::SubmitLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight)
{
	Line l = { p1, p2, colour, weight };
	m_Lines.push_back(l);
}
