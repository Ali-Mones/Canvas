#include "Camera.h"

void Camera::MoveLeft()
{
	m_Transform.x += 10;
	RecalculateView();
}

void Camera::MoveRight()
{
	m_Transform.x -= 10;
	RecalculateView();
}

void Camera::MoveUp()
{
	m_Transform.y -= 10;
	RecalculateView();
}

void Camera::MoveDown()
{
	m_Transform.y += 10;
	RecalculateView();
}

void Camera::RecalculateView()
{
	m_View = glm::translate(glm::mat4(1.0f), m_Transform);
}
