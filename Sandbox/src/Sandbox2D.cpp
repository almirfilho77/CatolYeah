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
	m_backgroundTexture = CatolYeah::Texture2D::Create("assets/textures/Checkerboard.png");
	m_zeca = CatolYeah::Texture2D::Create("assets/textures/zeca.png");
	m_carinha = CatolYeah::Texture2D::Create("assets/textures/carinha.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
	ImGui::ColorEdit4("Bar color", glm::value_ptr(m_barColor));
	ImGui::ColorEdit4("BG tinting color", glm::value_ptr(m_bgTintingColor));

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

	//CY_DEBUG("Square position [x:{0}] [y:{1}", m_squarePosition.x, m_squarePosition.y);

	CatolYeah::Renderer2D::DrawQuad({ -0.5f, -0.5f, -0.1f }, {0.1f, 0.1f}, m_carinha);										// Carinha
	CatolYeah::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, {10.0f, 10.0f}, m_backgroundTexture, m_bgTintingColor, 10.0f);	// Checkerboard

	CatolYeah::Renderer2D::DrawQuad({ -1.0f,  0.0f, 0.1f }, {0.4f, 0.4f}, m_squareColor);	// Setting Z-axis with depth test enabled
	CatolYeah::Renderer2D::DrawQuad({  0.5f, -0.5f, 0.1f }, {0.2f, 0.3f}, m_barColor);		// Setting Z-axis with depth test enabled

	//TODO: investigate why the alpha channel is not drawn as black color if the draw call is here, 
	// but if I move it 5 lines up I get black where it should be transparent
	CatolYeah::Renderer2D::DrawQuad({ m_squarePosition.x, m_squarePosition.y, 0.2f }, {0.5f, 0.5f}, m_zeca);				// Zeca
	static float rotation = 0;
	rotation += ts * 50.0f;
	CatolYeah::Renderer2D::DrawRotatedQuad({ -m_squarePosition.x, -m_squarePosition.y, 0.3f }, {0.5f, 0.5f}, glm::radians(rotation), m_zeca);
	
	CatolYeah::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(CatolYeah::Event& e)
{
	m_cameraController.OnEvent(e);
}
