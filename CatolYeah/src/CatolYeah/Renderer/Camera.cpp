#include "cypch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CatolYeah
{

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float top, float bottom)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{

	}

	void OrthographicCamera::SetPosition(float x, float y, float z)
	{
		m_position = glm::vec3({ x,y,z });
		m_RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_position = position;
		m_RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::SetRotation(float angle)
	{
		// TODO: adjust the range to fit -360º ~ 360º if this condition is met
		if (angle < -360.0f || angle > 360.0f)
		{
			CY_CORE_ERROR("Invalid rotation angle");
			return;
		}

		m_rotation = angle;
		m_RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::m_RecalculateViewProjectionMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}