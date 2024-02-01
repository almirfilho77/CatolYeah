#include <CatolYeah.h>
#include <CatolYeah/EntryPoint.h>
#include <iostream>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BlockBall.h"

class DebugLayer : public CatolYeah::Layer
{
public:
	DebugLayer()
		: Layer("DebugLayer") {}

	void OnAttach() override
	{
		CY_TRACE("DebugLayer::OnAttach!");
	}

	void OnImGuiRender() override
	{
		std::string gpu_info = CatolYeah::Application::Get().GetWindow().GetGPUInfo();
		ImGui::Begin("GPU Info");
		ImGui::Text(gpu_info.c_str());
		ImGui::End();
	}
};

class TestLayer : public CatolYeah::Layer
{
public:
	TestLayer()
		:	Layer("Test Layer"), m_aspectRatio(16.0f / 9.0f), m_cameraController(m_aspectRatio)
	{
		// Camera creation
		auto width = CatolYeah::Application::Get().GetWindow().GetWidth();
		auto height = CatolYeah::Application::Get().GetWindow().GetHeight();
		m_cameraController.SetAspectRatio((float)width / (float)height);
		m_squarePosition = { 0.0f, 0.0f, 0.0f };

		// Square
		m_squareVertexArray = CatolYeah::VertexArray::Create();

		float square_vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		CatolYeah::Ref<CatolYeah::VertexBuffer> squareVB;
		squareVB = CatolYeah::VertexBuffer::Create(square_vertices, sizeof(square_vertices));
		CatolYeah::VertexBufferLayout square_layout = {
			{ CatolYeah::ShaderDataType::Float3, "a_Position" },
			{ CatolYeah::ShaderDataType::Float2, "a_TextureCoord" },
		};
		squareVB->SetBufferLayout(square_layout);
		m_squareVertexArray->AddVertexBuffer(squareVB);

		uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
		CatolYeah::Ref<CatolYeah::IndexBuffer> squareIB;
		squareIB = CatolYeah::IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
		m_squareVertexArray->SetIndexBuffer(squareIB);

		std::string square_vertex_shader = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_ModelMatrix;
			uniform vec4 u_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
				v_Color = u_Color;	
			}
		)";

		std::string square_fragment_shader = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_squareShader = CatolYeah::Shader::Create("Squares", square_vertex_shader, square_fragment_shader);

		// Texture Shader
		m_shaderLib.Load("assets/shaders/Texture.glsl");
		//m_ronaldinhoTexture = CatolYeah::Texture2D::Create("assets/textures/ronaldinho.png");
		m_fishTankTexture = CatolYeah::Texture2D::Create("assets/textures/fishTank.png");
		//m_solidBGTexture = CatolYeah::Texture2D::Create("assets/textures/solid_bg.jpg");
		auto textureShader = m_shaderLib.Get("Texture");
		textureShader->Bind();
		textureShader->SetUniform1i("u_Texture", m_fishTankTexture->GetSlot());

		CatolYeah::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	virtual void OnEvent(CatolYeah::Event& e) override
	{
		m_cameraController.OnEvent(e);
	}

	virtual void OnUpdate(CatolYeah::Timestep ts) override
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
		CatolYeah::RenderCommand::Clear();

		CatolYeah::Renderer::BeginScene(m_cameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		m_squareShader->Bind();
		m_squareShader->SetUniform4f("u_Color", m_squareColor);

		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				auto pos = glm::vec3(m_squarePosition.x + x * 0.11f, 
									 m_squarePosition.y + y * 0.11f,
									 0.0f);
				auto squareTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
				CatolYeah::Renderer::Submit(m_squareShader, m_squareVertexArray, squareTransform);
			}
		}

		glm::mat4 imagesTransform = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		CatolYeah::Ref<CatolYeah::Shader> textureShader = m_shaderLib.Get("Texture");
		//m_solidBGTexture->Bind(0);
		//CatolYeah::Renderer::Submit(textureShader, m_squareVertexArray, imagesTransform);
		m_fishTankTexture->Bind(0);
		CatolYeah::Renderer::Submit(textureShader, m_squareVertexArray, imagesTransform);

		CatolYeah::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}
	
private:
	CatolYeah::OrthographicCameraController m_cameraController;
	float m_aspectRatio;

	CatolYeah::Ref<CatolYeah::VertexArray> m_triangleVertexArray;
	CatolYeah::Ref<CatolYeah::Shader> m_triangleShader;

	CatolYeah::Ref<CatolYeah::VertexArray> m_squareVertexArray;
	CatolYeah::Ref<CatolYeah::Shader> m_squareShader;

	CatolYeah::ShaderLibrary m_shaderLib;
	CatolYeah::Ref<CatolYeah::Texture2D> m_fishTankTexture;
	CatolYeah::Ref<CatolYeah::Texture2D> m_solidBGTexture;

	glm::vec4 m_squareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec3 m_squarePosition;
	float m_squareTranslationSpeed = 5.0f;

};

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		CY_TRACE("Sandbox c'tor");
		//PushLayer(new TestLayer());
		PushLayer(new BlockBall::BlockBall());
		PushLayer(new DebugLayer());
	}

	~Sandbox()
	{
		CY_TRACE("Sandbox d'tor");
	}
};

CatolYeah::Application* CatolYeah::CreateApplication()
{
	return new Sandbox();
}