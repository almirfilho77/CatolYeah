#include "BlockBall.h"

namespace BlockBall
{
	Rect::Rect(const glm::vec3 &position, const glm::vec4 &color)
		:	m_position(position),
			m_color(color)
	{
		m_vertexArray = CatolYeah::VertexArray::Create();

		// Vertex Buffer
		float rect_vertices[4 * 2] = {
			-0.15f, -0.25f,
			 0.15f, -0.25f,
			 0.15f,  0.25f,
			-0.15f,  0.25f,
		};
		auto vertex_buffer = CatolYeah::VertexBuffer::Create(rect_vertices, sizeof(rect_vertices));
		CatolYeah::VertexBufferLayout buffer_layout = {
			{ CatolYeah::ShaderDataType::Float2, "a_Position" }
		};
		vertex_buffer->SetBufferLayout(buffer_layout);
		m_vertexArray->AddVertexBuffer(vertex_buffer);

		// Index Buffer
		uint32_t indices[6] = { 0, 1, 2, 2, 0, 3 };
		auto index_buffer = CatolYeah::IndexBuffer::Create(indices, 6);
		m_vertexArray->SetIndexBuffer(index_buffer);

		// Shader
		m_shader = CatolYeah::Shader::Create("assets/shaders/SolidColor.glsl");
		m_shader->Bind();
		m_shader->SetUniform4f("u_Color", m_color);
	}

	void Rect::SetPosition(float position_y)
	{
		m_position.y = position_y;
	}


	BlockBall::BlockBall()
		:	Layer("BlockBall"),
			m_aspectRatio(16.0f / 9.0f),
			m_cameraController(m_aspectRatio)
	{
		auto win_width = CatolYeah::Application::Get().GetWindow().GetWidth();
		auto win_height = CatolYeah::Application::Get().GetWindow().GetHeight();
		m_aspectRatio = (float)win_width / (float)win_height;
		m_cameraController.SetAspectRatio(m_aspectRatio);

		m_playerA = Rect({ -m_aspectRatio+0.15, 0.25, 0.0f }, { 0.8f, 0.2f, 0.2f, 1.0f });
		m_playerB = Rect({ m_aspectRatio-0.15, 0.25, 0.0f }, { 0.2f, 0.2f, 0.8f, 1.0f });
	}

	void BlockBall::OnUpdate(CatolYeah::Timestep ts)
	{
		// Square translation
		auto &playerA_position = m_playerA.GetPosition();
		if (playerA_position.y < 0.75f && CatolYeah::Input::IsKeyPressed(CY_KEY_W))
			m_playerA.SetPosition(playerA_position.y + m_rectTranslationSpeed * ts);
		else if (playerA_position.y > -0.75f && CatolYeah::Input::IsKeyPressed(CY_KEY_S))
			m_playerA.SetPosition(playerA_position.y - m_rectTranslationSpeed * ts);

		auto& playerB_position = m_playerB.GetPosition();
		if (playerB_position.y < 0.75f && CatolYeah::Input::IsKeyPressed(CY_KEY_O))
			m_playerB.SetPosition(playerB_position.y + m_rectTranslationSpeed * ts);
		else if (playerB_position.y > -0.75f && CatolYeah::Input::IsKeyPressed(CY_KEY_L))
			m_playerB.SetPosition(playerB_position.y - m_rectTranslationSpeed * ts);

		//CY_INFO("Aspect Ratio: {0}", m_aspectRatio);
		//CY_INFO("Player A position Y: {0}", m_playerA.GetPosition().y);
		//CY_INFO("Player B position Y: {0}", m_playerB.GetPosition().y);

		auto playerA_transform = glm::translate(glm::mat4(1.0f), m_playerA.GetPosition());
		auto playerB_transform = glm::translate(glm::mat4(1.0f), m_playerB.GetPosition());

		// Render commands
		CatolYeah::RenderCommand::Clear();
		CatolYeah::Renderer::BeginScene(m_cameraController.GetCamera());

		CatolYeah::Renderer::Submit(m_playerA.GetShader(), m_playerA.GetVertexArray(), playerA_transform);
		CatolYeah::Renderer::Submit(m_playerB.GetShader(), m_playerB.GetVertexArray(), playerB_transform);

		CatolYeah::Renderer::EndScene();
	}

	void BlockBall::OnEvent(CatolYeah::Event& e)
	{
		m_cameraController.OnEvent(e);
	}
	
}// BlockBall