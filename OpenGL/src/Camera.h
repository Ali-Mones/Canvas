#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	int Left() { return -m_Transform.x; }
	int Bottom() { return -m_Transform.y; }

	glm::mat4 View() { return m_View; }
private:
	void RecalculateView();
private:
	glm::vec3 m_Transform = glm::vec3(0);
	glm::mat4 m_View = glm::translate(glm::mat4(1.0f), m_Transform);
};

