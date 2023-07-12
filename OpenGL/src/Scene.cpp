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
	{
		m_Shapes[0].Position.x -= 10;
	}

	if (KeyboardInput(GLFW_KEY_RIGHT))
	{
		m_Shapes[0].Position.x += 10;
	}

	if (KeyboardInput(GLFW_KEY_UP))
	{
		m_Shapes[0].Position.y += 10;
	}

	if (KeyboardInput(GLFW_KEY_DOWN))
	{
		m_Shapes[0].Position.y -= 10;
	}

	if (MouseInput(GLFW_MOUSE_BUTTON_LEFT))
	{
		double xpos, ypos;
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		glfwGetCursorPos(m_Window, &xpos, &ypos);
		Quad q({ (float) xpos, (float) height - ypos, 0 }, { 100, 100, 0 }, { 1, 1, 1, 1 });
		m_Shapes.push_back(q);
	}
}

void Scene::OnRender()
{
	Renderer::Clear(m_ClearColour);
	Renderer::BatchStart();

	for (auto& shape : m_Shapes)
		Renderer::RenderQuad(shape);

	Renderer::BatchEnd();
	Renderer::Flush();
}

void Scene::OnImGuiRender()
{
	ImGui::Begin("IDK");

	for (auto& shape : m_Shapes)
	{
		int index = &shape - &m_Shapes[0];
		std::string positionLabel = "shape " + std::to_string(index + 1) + " position";
		std::string colourLabel = "shape " + std::to_string(index + 1) + " colour";
		ImGui::DragFloat3(positionLabel.c_str(), &shape.Position.x);
		ImGui::ColorEdit4(colourLabel.c_str(), &shape.Colour.r);
	}

	ImGui::End();
}

void Scene::SubmitQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation)
{
	Quad quad = { pos, dims, colour, rotation };
	m_Shapes.push_back(quad);
}
