#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(float windowWidth, float windowHeight);

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void ZoomIn();
	void ZoomOut();

	glm::vec4 GetWorldPosition(float x, float y, float width, float height);

	void SetProjection(float left, float right, float bottom, float top) { m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); }

	const glm::mat4& View() const { return m_View; }
	const glm::mat4& Projection() const { return m_Projection; }
	const glm::mat4& ViewProjection() const { return m_ViewProjection; }
private:
	void RecalculateView();
	void RecalculateProjection();
private:
	float m_AspectRatio;
	float m_ZoomSpeed = 5.0f;
	float m_Left, m_Right, m_Bottom, m_Top;

	glm::vec3 m_Translation;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	glm::mat4 m_ViewProjection;
};

