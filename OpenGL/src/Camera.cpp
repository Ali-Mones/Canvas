#include "Camera.h"
#include <iostream>

Camera::Camera(float windowWidth, float windowHeight)
	: m_Transform(glm::vec3(0)), m_View(glm::translate(glm::mat4(1.0f), m_Transform)),
		m_Projection(glm::ortho(0.0f, windowWidth, 0.0f, windowHeight, -1.0f, 1.0f)), m_ViewProjection(m_Projection * m_View),
		m_AspectRatio(windowWidth / windowHeight), m_Left(0.0f), m_Right(windowWidth), m_Bottom(0.0f), m_Top(windowHeight)
{
}

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

void Camera::ZoomIn()
{
	float zoomInLimit = m_ZoomSpeed * 5.0;
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
	m_View = glm::translate(glm::mat4(1.0f), m_Transform);
	m_ViewProjection = m_Projection * m_View;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, -1.0f, 1.0f);
	m_ViewProjection = m_Projection * m_View;
}
