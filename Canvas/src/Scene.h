#pragma once
#include "Shape.h"
#include "Camera.h"
#include <vector>

class Scene
{
public:
	Scene();
	void OnUpdate();
	void SubmitRect(int x, int y, int w, int h, glm::vec4 colour);
	void SubmitCircle(int x, int y, int w, int h, glm::vec4 colour);
	void SubmitLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 colour, float weight);
private:
	void OnInputUpdate();
	void OnRender();
	void OnImGuiRender();
private:
	Camera& m_Camera;
	std::vector<Rect> m_Rects;
	std::vector<Circle> m_Circles;
	std::vector<Line> m_Lines;
	glm::vec4 m_ClearColour = { 1.0f, 1.0f, 1.0f, 1.0f };
};
