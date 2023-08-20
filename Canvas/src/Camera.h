#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core.h"

class API Camera
{
public:
	static Camera& Get();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void ZoomIn();
	void ZoomOut();

	glm::vec4 GetWorldPosition(float x, float y, float width, float height);
	Camera(const Camera&) = delete;
private:
	Camera(float windowWidth, float windowHeight);
	static Camera& Init(float windowWidth, float windowHeight);
	void OnWindowResize(float windowWidth, float windowHeight);
	void RecalculateView();
	void RecalculateProjection();
	const glm::mat4& View() const { return m_View; }
	const glm::mat4& Projection() const { return m_Projection; }
	const glm::mat4& ViewProjection() const { return m_ViewProjection; }
private:
	float m_AspectRatio;
	float m_ZoomSpeed = 5.0f;
	float m_ZoomLevel = 1.0f;
	float m_WindowWidth, m_WindowHeight;

	glm::vec3 m_Translation;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::mat4 m_ViewProjection;

	friend class Application;
};

