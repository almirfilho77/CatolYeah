#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
	:	Layer("Sandbox2D"), m_cameraController(16.0f/9.0f, false)
{
	m_squarePosition = { 0.0f, 0.0f, 0.0f };
}

void Sandbox2D::OnAttach()
{
	// Camera creation
	auto width = CatolYeah::Application::Get().GetWindow().GetWidth();
	auto height = CatolYeah::Application::Get().GetWindow().GetHeight();
	m_cameraController.SetAspectRatio((float)width / (float)height);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::ColorEdit4("Bar color", glm::value_ptr(m_barColor));

	ImGui::End();
}

void Sandbox2D::OnUpdate(CatolYeah::Timestep ts)
{
	//CY_INFO("Timestep: {0}", ts.GetSeconds());
	m_cameraController.OnUpdate(ts);

	// Square translation
	if (CatolYeah::Input::IsKeyPressed(CY_KEY_L))
		m_squarePosition.x += m_squareTranslationSpeed * ts;
	else if (CatolYeah::Input::IsKeyPressed(CY_KEY_J))
		m_squarePosition.x -= m_squareTranslationSpeed * ts;

	if (CatolYeah::Input::IsKeyPressed(CY_KEY_I))
		m_squarePosition.y += m_squareTranslationSpeed * ts;
	else if (CatolYeah::Input::IsKeyPressed(CY_KEY_K))
		m_squarePosition.y -= m_squareTranslationSpeed * ts;

	// Render commands
	CatolYeah::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	CatolYeah::RenderCommand::Clear();

	CatolYeah::Renderer2D::BeginScene(m_cameraController.GetCamera());

	//CY_INFO("Square position [x:{0}] [y:{1}", m_squarePosition.x, m_squarePosition.y);
	CatolYeah::Renderer2D::DrawQuad({ m_squarePosition.x, m_squarePosition.y }, {1.0f, 1.0f}, m_squareColor);
	CatolYeah::Renderer2D::DrawQuad({ m_squarePosition.x-0.75f, m_squarePosition.y-0.5f }, {0.2f, 2.0f}, m_barColor);
	CatolYeah::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(CatolYeah::Event& e)
{
	m_cameraController.OnEvent(e);
}
