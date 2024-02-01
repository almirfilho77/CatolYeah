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
		OrthographicCamera() = delete;
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		OrthographicCamera(	const OrthographicCamera &other) = default;

		~OrthographicCamera();

		inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		inline const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);

		inline float GetRotation() const { return m_rotation; }
		void SetRotation(float angle);

	private:
		void m_RecalculateViewProjectionMatrix();

	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;
		
		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
	};

}