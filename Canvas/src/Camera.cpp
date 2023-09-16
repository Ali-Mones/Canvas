#include "Camera.h"
#include <iostream>
#include <glew.h>

static bool s_Init = false;

Camera::Camera(float windowWidth, float windowHeight)
	: m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), m_AspectRatio(windowWidth / windowHeight), m_ZoomLevel(windowHeight / 2),
		m_Translation(glm::vec3(0)), m_View(glm::translate(glm::mat4(1.0f), m_Translation)),
		m_Projection(glm::ortho(-m_AspectRatio * m_ZoomLevel + windowWidth / 2, m_AspectRatio * m_ZoomLevel + windowWidth / 2, -m_ZoomLevel + windowHeight / 2, m_ZoomLevel + windowHeight / 2, -1.0f, 1.0f)),
		m_ViewProjection(m_Projection * m_View)
{
}

Camera& Camera::Init(float windowWidth, float windowHeight)
{
	s_Init = true;
	static Camera instance(windowWidth, windowHeight);
	return instance;
}

Camera& Camera::Get()
{
	if (!s_Init)
		std::cout << "NO CAMERA FOUND\nPLEASE USE CAMERA::INIT() FIRST\n";
	else
		return Init(0, 0);
}

void Camera::MoveLeft(float speed)
{
	m_Translation.x -= speed;
	RecalculateView();
}

void Camera::MoveRight(float speed)
{
	m_Translation.x += speed;
	RecalculateView();
}

void Camera::MoveUp(float speed)
{
	m_Translation.y += speed;
	RecalculateView();
}

void Camera::MoveDown(float speed)
{
	m_Translation.y -= speed;
	RecalculateView();
}

void Camera::ZoomIn(float speed)
{
	m_ZoomLevel -= speed;
	m_ZoomLevel = std::max(0.25f, m_ZoomLevel);
	RecalculateProjection();
}

void Camera::ZoomOut(float speed)
{
	m_ZoomLevel += speed;
	RecalculateProjection();
}

glm::vec4 Camera::GetWorldPosition(float x, float y)
{
	glm::vec4 clip((float) x * 2.0f / m_WindowWidth - 1.0f, (float) (m_WindowHeight - y) * 2.0f / m_WindowHeight - 1.0f, 0.0f, 1.0f);
	glm::vec4 model = glm::inverse(ViewProjection()) * clip;
	return model;
}

void Camera::OnWindowResize(float windowWidth, float windowHeight)
{
	if (windowWidth != m_WindowWidth || windowHeight != m_WindowHeight)
	{
		m_ZoomLevel -= m_WindowHeight / 2;
		m_ZoomLevel += windowHeight / 2;

		m_WindowWidth = windowWidth;
		m_WindowHeight = windowHeight;
		m_AspectRatio = m_WindowWidth / m_WindowHeight;

		glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		RecalculateProjection();
	}
}

void Camera::RecalculateView()
{
	m_View = glm::translate(glm::mat4(1.0f), m_Translation);
	m_View = glm::inverse(m_View);
	m_ViewProjection = m_Projection * m_View;
}

void Camera::RecalculateProjection()
{
	m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel + m_WindowWidth / 2, m_AspectRatio * m_ZoomLevel + m_WindowWidth / 2,
							  -m_ZoomLevel + m_WindowHeight / 2, m_ZoomLevel + m_WindowHeight / 2,
							  -1.0f, 1.0f);
	m_ViewProjection = m_Projection * m_View;
}
