#include "Camera.h"
#include <iostream>

Camera::Camera(float windowWidth, float windowHeight)
	: m_Translation(glm::vec3(0)), m_View(glm::translate(glm::mat4(1.0f), m_Translation)),
		m_Projection(glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, -1.0f, 1.0f)), m_ViewProjection(m_Projection * m_View),
		m_AspectRatio(windowWidth / windowHeight), m_Left(0.0f), m_Right(windowWidth), m_Bottom(0.0f), m_Top(windowHeight)
{
}

void Camera::MoveLeft()
{
	m_Translation.x -= 10;
	RecalculateView();
}

void Camera::MoveRight()
{
	m_Translation.x += 10;
	RecalculateView();
}

void Camera::MoveUp()
{
	m_Translation.y += 10;
	RecalculateView();
}

void Camera::MoveDown()
{
	m_Translation.y -= 10;
	RecalculateView();
}

void Camera::ZoomIn()
{
	float zoomInLimit = m_ZoomSpeed * 2.0;
	if (m_Left + zoomInLimit < m_Right && m_Bottom + zoomInLimit < m_Top)
	{
		m_Left += m_AspectRatio * m_ZoomSpeed;
		m_Right -= m_AspectRatio * m_ZoomSpeed;
		m_Bottom += m_ZoomSpeed;
		m_Top -= m_ZoomSpeed;
	}

	RecalculateProjection();
}

void Camera::ZoomOut()
{
	m_Left -= m_AspectRatio * m_ZoomSpeed;
	m_Right += m_AspectRatio * m_ZoomSpeed;
	m_Bottom -= m_ZoomSpeed;
	m_Top += m_ZoomSpeed;

	RecalculateProjection();
}

void Camera::RecalculateView()
{
	m_View = glm::translate(glm::mat4(1.0f), m_Translation);
	m_View = glm::inverse(m_View);
	m_ViewProjection = m_Projection * m_View;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, -1.0f, 1.0f);
	m_ViewProjection = m_Projection * m_View;
}

glm::vec4 Camera::GetWorldPosition(float x, float y, float width, float height)
{
	glm::vec4 clip((float) x * 2.0f / width - 1.0f, (float) (height - y) * 2.0f / height - 1.0f, 0.0f, 1.0f);
	glm::vec4 model = glm::inverse(ViewProjection()) * clip;
	return model;
}
