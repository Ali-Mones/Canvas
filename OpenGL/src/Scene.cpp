#include "Scene.h"
#include "Renderer.h"
#include <imgui/imgui.h>

Scene::Scene(GLFWwindow* window)
	: m_Window(window)
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
		m_Quads[0].Position.x -= 10;

	if (KeyboardInput(GLFW_KEY_D))
		m_Quads[0].Position.x += 10;

	if (KeyboardInput(GLFW_KEY_S))
		m_Quads[0].Position.y -= 10;

	if (KeyboardInput(GLFW_KEY_W))
		m_Quads[0].Position.y += 10;

	if (MouseInput(GLFW_MOUSE_BUTTON_LEFT))
	{
		double xpos, ypos;
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		glfwGetCursorPos(m_Window, &xpos, &ypos);
		SubmitQuad({ (float) m_Camera.Left() + xpos, (float) m_Camera.Bottom() + height - ypos, 0 }, { 100, 100, 0 }, { 1, 1, 1, 1 });
	}
}

void Scene::OnRender()
{
	Renderer::Clear(m_ClearColour);
	Renderer::StartBatch();

	for (auto& quad : m_Quads)
		Renderer::RenderFilledQuad(quad.Position, quad.Dimensions, quad.Colour, quad.Rotation, 10.0f);

	for (auto& circle : m_Circles)
		Renderer::RenderCircle(circle.Position, circle.Dimensions, circle.Colour, circle.Thickness, circle.Fade, 10.0f);

	for (auto& line : m_Lines)
		Renderer::RenderLine(line);

	Renderer::RenderHollowQuad(glm::vec3(0, 0, 0), glm::vec3(100, 100, 0), glm::vec4(0.5), 0.1);

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

void Scene::SubmitQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation)
{
	Quad quad = { pos, dims, colour, rotation };
	m_Quads.push_back(quad);
}

void Scene::SubmitCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness, float fade)
{
	Circle c = { pos, dims, colour, thickness, fade };
	m_Circles.push_back(c);
}

void Scene::SubmitLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight)
{
	Line l = { p1, p2, colour, weight };
	m_Lines.push_back(l);
}
