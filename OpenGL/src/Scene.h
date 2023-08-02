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
	void SubmitCircle(glm::vec3 pos, glm::vec3 dims, glm::vec4 colour, float thickness = 1.0f, float fade = 0.02f);
	void SubmitLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight);
private:
	void OnInputUpdate();
	void OnRender();
	void OnImGuiRender();
private:
	GLFWwindow* m_Window;
	std::vector<Quad> m_Quads;
	std::vector<Circle> m_Circles;
	std::vector<Line> m_Lines;
	glm::vec4 m_ClearColour = { 0.0f, 0.0f, 0.0f, 1.0f };
};
