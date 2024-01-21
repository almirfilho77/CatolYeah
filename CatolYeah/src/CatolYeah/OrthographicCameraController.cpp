#include "cypch.h"
#include "OrthographicCameraController.h"

#include "CatolYeah/Input.h"
#include "CatolYeah/KeyCodes.h"

#define ZOOM_LEVEL_TO_TRANSLATION_SPEED 0.25f

namespace CatolYeah
{
	OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool enable_rotation)
		:	m_aspectRatio(aspect_ratio),
			m_enableRotation(enable_rotation),
			m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel)
	{
		m_cameraTranslationSpeed = m_zoomLevel * ZOOM_LEVEL_TO_TRANSLATION_SPEED;
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(CY_BIND_EVENT_FN(OrthographicCameraController::m_OnWindowResize));
		dispatcher.Dispatch<MouseScrolledEvent>(CY_BIND_EVENT_FN(OrthographicCameraController::m_OnMouseScrolled));
	}

	void OrthographicCameraController::OnUpdate(float ts)
	{
		// Camera translation
		if (CatolYeah::Input::IsKeyPressed(CY_KEY_D))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;

		if (CatolYeah::Input::IsKeyPressed(CY_KEY_W))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;
		m_camera.SetPosition(m_cameraPosition);

		// Camera rotation
		if (m_enableRotation)
		{
			if (CatolYeah::Input::IsKeyPressed(CY_KEY_Q))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			else if (CatolYeah::Input::IsKeyPressed(CY_KEY_E))
				m_cameraRotation -= m_cameraRotationSpeed * ts;
			m_camera.SetRotation(m_cameraRotation);
		}
	}

	bool OrthographicCameraController::m_OnWindowResize(WindowResizeEvent& e)
	{
		m_aspectRatio = (float)e.GetWindowWidth() / (float)e.GetWindowHeight();
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return EVENT_RETURN_PASS_ON;
	}

	bool OrthographicCameraController::m_OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.GetYOffset();
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		m_cameraTranslationSpeed = m_zoomLevel * ZOOM_LEVEL_TO_TRANSLATION_SPEED;
		return EVENT_RETURN_PASS_ON;
	}
}