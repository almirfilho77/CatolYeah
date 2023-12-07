#include "cypch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CatolYeah
{

	OrthographicCamera::OrthographicCamera()
		:	m_position( { 0.0f, 0.0f, 0.0f } ),
			m_zAxisRotation(DEFAULT_CAMERA_ROTATION),
			m_width(DEFAULT_CAMERA_WIDTH),
			m_height(DEFAULT_CAMERA_HEIGHT)
	{
		m_projectionMatrix = glm::ortho(0.0f, m_width, 0.0f, m_height, -1.0f, 1.0f);
		m_RecalculateViewProjectionMatrix();
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:	m_position({ 0.0f, 0.0f, 0.0f }),
			m_zAxisRotation(DEFAULT_CAMERA_ROTATION)
	{
		if (left > right || bottom > top)
		{
			CY_CORE_ERROR("Invalid values for the camera view size. Applying default width and height values ({0}x{1})", DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_HEIGHT);
			m_width = DEFAULT_CAMERA_WIDTH;
			m_height = DEFAULT_CAMERA_HEIGHT;
			m_projectionMatrix = glm::ortho(0.0f, DEFAULT_CAMERA_WIDTH, 0.0f, DEFAULT_CAMERA_HEIGHT, -1.0f, 1.0f);
		}
		else
		{
			CY_CORE_DEBUG("Projection matrix args: left[{0}],right[{1}],bottom[{2}],top[{3}]",
				left, right, bottom, top);
			m_width = right - left;
			m_height = top - bottom;
			m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		}

		m_RecalculateViewProjectionMatrix();
	}

	OrthographicCamera::OrthographicCamera(const glm::vec3& position)
		:	m_position(position),
			m_zAxisRotation(DEFAULT_CAMERA_ROTATION),
			m_width(DEFAULT_CAMERA_WIDTH),
			m_height(DEFAULT_CAMERA_HEIGHT)
	{
		m_projectionMatrix = glm::ortho(0.0f, m_width, 0.0f, m_height, -1.0f, 1.0f);
		m_RecalculateViewProjectionMatrix();
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

		m_zAxisRotation = angle;
		m_RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::SetWidth(float width)
	{
		if (width < 0.0f)
		{
			CY_CORE_ERROR("Invalid value for camera width");
		}

		m_width = width;
	}

	void OrthographicCamera::SetHeight(float height)
	{
		if (height < 0.0f)
		{
			CY_CORE_ERROR("Invalid value for camera height");
		}

		m_height = height;
	}

	void OrthographicCamera::m_RecalculateViewProjectionMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_zAxisRotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);

		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}