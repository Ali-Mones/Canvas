#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core.h"

class CANVAS_API Camera
{
public:
	static Camera& Init(float windowWidth, float windowHeight);
	static Camera& Get();

	void OnWindowResize(float windowWidth, float windowHeight);
	void RecalculateView();
	void RecalculateProjection();

	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

	void ZoomIn(float speed);
	void ZoomOut(float speed);

	glm::vec4 GetWorldPosition(float x, float y);

	const glm::mat4& View() const { return m_View; }
	const glm::mat4& Projection() const { return m_Projection; }
	const glm::mat4& ViewProjection() const { return m_ViewProjection; }

	Camera(const Camera&) = delete;
private:
	Camera(float windowWidth, float windowHeight);
private:
	float m_AspectRatio;
	float m_ZoomLevel;
	float m_WindowWidth, m_WindowHeight;

	glm::vec3 m_Translation;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::mat4 m_ViewProjection;
};

