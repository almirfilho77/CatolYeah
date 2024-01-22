#pragma once

#include "CatolYeah/Renderer/Camera.h"
#include "CatolYeah/Events/MouseEvent.h"
#include "CatolYeah/Events/ApplicationEvent.h"

namespace CatolYeah
{
	
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspect_ratio, bool enable_rotation = false);

		void OnEvent(Event& e);
		void OnUpdate(float ts);
		void SetAspectRatio(float aspectRatio);

		inline OrthographicCamera& GetCamera() { return m_camera; }
		inline const OrthographicCamera& GetCamera() const { return m_camera; }

	private:
		bool m_OnWindowResize(WindowResizeEvent& e);
		bool m_OnMouseScrolled(MouseScrolledEvent& e);

	private:
		float m_aspectRatio;
		bool m_enableRotation;

		float m_zoomLevel = 1.0f;
		OrthographicCamera m_camera;

		float m_cameraTranslationSpeed = 5.0f;
		float m_cameraRotationSpeed = 180.0f;
		float m_cameraRotation = 0.0f;

		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
	};

}//CatolYeah