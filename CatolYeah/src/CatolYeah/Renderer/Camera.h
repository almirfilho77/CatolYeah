#pragma once

#include <glm/glm.hpp>

#define DEFAULT_CAMERA_FOV		45.0f
#define DEFAULT_CAMERA_ROTATION	0.0f
#define DEFAULT_CAMERA_WIDTH	960.0f
#define DEFAULT_CAMERA_HEIGHT	540.0f

namespace CatolYeah
{
	
	class OrthographicCamera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float left, float right, float top, float bottom);
		OrthographicCamera(const glm::vec3 &position);

		OrthographicCamera(	const OrthographicCamera &other) = default;

		~OrthographicCamera();

		inline const glm::vec3& GetPosition() const { return m_position; }

		inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		inline float GetRotation() const { return m_zAxisRotation; }
		inline float GetWidth() const { return m_width; }
		inline float GetHeight() const { return m_height; }

		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		void SetRotation(float angle);

		void SetWidth(float width);
		void SetHeight(float height);

	private:
		void m_RecalculateViewProjectionMatrix();

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position;

		float m_zAxisRotation;
		float m_width, m_height;
	};

}