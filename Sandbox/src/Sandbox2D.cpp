#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
	:	Layer("Sandbox2D"), m_cameraController(16.0f/9.0f, false)
{
}

void Sandbox2D::OnAttach()
{
	// Camera creation
	auto width = CatolYeah::Application::Get().GetWindow().GetWidth();
	auto height = CatolYeah::Application::Get().GetWindow().GetHeight();
	m_cameraController.SetAspectRatio((float)width / (float)height);
	m_squarePosition = { 0.0f, 0.0f, 0.0f };

	// Square
	m_squareVAO = CatolYeah::VertexArray::Create();

	float square_vertices[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};
	CatolYeah::Ref<CatolYeah::VertexBuffer> squareVB;
	squareVB = CatolYeah::VertexBuffer::Create(square_vertices, sizeof(square_vertices));
	CatolYeah::VertexBufferLayout square_layout = {
		{ CatolYeah::ShaderDataType::Float3, "a_Position" },
	};
	squareVB->SetBufferLayout(square_layout);
	m_squareVAO->AddVertexBuffer(squareVB);

	uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
	CatolYeah::Ref<CatolYeah::IndexBuffer> squareIB;
	squareIB = CatolYeah::IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
	m_squareVAO->SetIndexBuffer(squareIB);

	m_solidColorShader = CatolYeah::Shader::Create("assets/shaders/SolidColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
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

	CatolYeah::Renderer::BeginScene(m_cameraController.GetCamera());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	m_solidColorShader->Bind();
	m_solidColorShader->SetUniform4f("u_Color", m_squareColor);

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			auto pos = glm::vec3(m_squarePosition.x + x * 0.11f,
				m_squarePosition.y + y * 0.11f,
				0.0f);
			auto squareTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
			CatolYeah::Renderer::Submit(m_solidColorShader, m_squareVAO, squareTransform);
		}
	}
	CatolYeah::Renderer::EndScene();
}

void Sandbox2D::OnEvent(CatolYeah::Event& e)
{
	m_cameraController.OnEvent(e);
}
