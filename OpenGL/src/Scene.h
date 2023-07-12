#pragma once
#include "Shape.h"
#include <vector>
#include <glfw3.h>

class Scene
{
public:
	Scene(GLFWwindow* window);
	bool KeyboardInput(int key);
	bool MouseInput(int button);
	void OnUpdate();
	void SubmitQuad(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float rotation = 0.0f);
private:
	void OnInputUpdate();
	void OnRender();
	void OnImGuiRender();
private:
	GLFWwindow* m_Window;
	std::vector<Quad> m_Shapes;
	glm::vec4 m_ClearColour = { 0.45, 0.55, 0.60, 1.00 };
};

