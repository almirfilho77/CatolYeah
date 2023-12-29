#include <CatolYeah.h>
#include <iostream>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DebugLayer : public CatolYeah::Layer
{
public:
	DebugLayer()
		: Layer("DebugLayer") {}

	void OnAttach() override
	{
		CY_TRACE("DebugLayer::OnAttach!");
	}

	void OnUpdate(CatolYeah::Timestep ts) override
	{
		
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
		: Layer("Test Layer")
	{
		// Camera creation
		auto right = CatolYeah::Application::Get().GetWindow().GetWidth();
		auto bottom = CatolYeah::Application::Get().GetWindow().GetHeight();
		m_camera = CatolYeah::OrthographicCamera(0.0f, (float)right, 0.0f, (float)bottom);
		m_cameraPosition = m_camera.GetPosition();
		m_cameraRotation = m_camera.GetRotation();
		m_squarePosition = { 0.0f, 0.0f, 0.0f };

		// Triangle
		m_triangleVertexArray = CatolYeah::VertexArray::Create();

		float triangle_vertices[3 * 7] = {
			 50.0f,  50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 150.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 100.0f, 150.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		CatolYeah::Ref<CatolYeah::VertexBuffer> triangleVB;
		triangleVB.reset(CatolYeah::VertexBuffer::Create(triangle_vertices, sizeof(triangle_vertices)));
		CatolYeah::VertexBufferLayout layout = {
			{ CatolYeah::ShaderDataType::Float3, "a_Position" },
			{ CatolYeah::ShaderDataType::Float4, "a_Color"}
		};
		triangleVB->SetBufferLayout(layout);
		m_triangleVertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		CatolYeah::Ref<CatolYeah::IndexBuffer> triangleIB;
		triangleIB.reset(CatolYeah::IndexBuffer::Create(indices, sizeof(triangle_vertices) / sizeof(uint32_t)));
		m_triangleVertexArray->SetIndexBuffer(triangleIB);

		std::string triangle_vertex_shader = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_ModelMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
				v_Color = a_Color;	
			}
		)";

		std::string triangle_fragment_shader = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_triangleShader = CatolYeah::Shader::Create("Triangle", triangle_vertex_shader, triangle_fragment_shader);

		// Square
		m_squareVertexArray = CatolYeah::VertexArray::Create();

		float square_vertices[4 * 5] = {
			25.0f,  25.0f, 0.0f, 0.0f, 0.0f, 
			175.0f, 25.0f, 0.0f, 1.0f, 0.0f, 
			175.0f, 175.0f, 0.0f, 1.0f, 1.0f,
			25.0f,  175.0f, 0.0f, 0.0f, 1.0f,
		};
		CatolYeah::Ref<CatolYeah::VertexBuffer> squareVB;
		squareVB.reset(CatolYeah::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
		CatolYeah::VertexBufferLayout square_layout = {
			{ CatolYeah::ShaderDataType::Float3, "a_Position" },
			{ CatolYeah::ShaderDataType::Float2, "a_TextureCoord" },
		};
		squareVB->SetBufferLayout(square_layout);
		m_squareVertexArray->AddVertexBuffer(squareVB);

		uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
		CatolYeah::Ref<CatolYeah::IndexBuffer> squareIB;
		squareIB.reset(CatolYeah::IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t)));
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
		//m_texture = CatolYeah::Texture2D::Create("assets/textures/ronaldinho.png");
		m_texture = CatolYeah::Texture2D::Create("assets/textures/fishTank.png");
		m_solidBGTexture = CatolYeah::Texture2D::Create("assets/textures/solid_bg.jpg");
		auto textureShader = m_shaderLib.Get("Texture");
		textureShader->Bind();
		textureShader->SetUniform1i("u_Texture", m_texture->GetSlot());

		CatolYeah::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	virtual void OnAttach() override
	{

	}

	virtual void OnUpdate(CatolYeah::Timestep ts) override
	{
		//CY_INFO("Timestep: {0}", ts.GetSeconds());

		// Camera translation
		if (CatolYeah::Input::IsKeyPressed(CY_KEY_RIGHT))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_LEFT))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;

		if (CatolYeah::Input::IsKeyPressed(CY_KEY_UP))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_DOWN))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

		// Camera rotation
		if (CatolYeah::Input::IsKeyPressed(CY_KEY_Q))
			m_cameraRotation += m_cameraRotationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_E))
			m_cameraRotation -= m_cameraRotationSpeed * ts;

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);

		// Square translation
		if (CatolYeah::Input::IsKeyPressed(CY_KEY_D))
			m_squarePosition.x += m_squareTranslationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_A))
			m_squarePosition.x -= m_squareTranslationSpeed * ts;

		if (CatolYeah::Input::IsKeyPressed(CY_KEY_W))
			m_squarePosition.y += m_squareTranslationSpeed * ts;
		else if (CatolYeah::Input::IsKeyPressed(CY_KEY_S))
			m_squarePosition.y -= m_squareTranslationSpeed * ts;
		
		// Render commands
		CatolYeah::RenderCommand::Clear();
		CatolYeah::Renderer::BeginScene(m_camera);

		auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		m_squareShader->Bind();
		m_squareShader->SetUniform4f("u_Color", m_squareColor);
		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				auto pos = glm::vec3(m_squarePosition.x + x * 20.0f, 
					m_squarePosition.y + y * 20.0f, 
					0.0f);
				auto squareTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
				CatolYeah::Renderer::Submit(m_squareShader, m_squareVertexArray, squareTransform);
			}
		}
		glm::mat4 imagesTransform = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 100.0f, 0.0f)) *
									glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 1.0f));
		m_solidBGTexture->Bind(0);
		auto textureShader = m_shaderLib.Get("Texture");
		CatolYeah::Renderer::Submit(textureShader, m_squareVertexArray, imagesTransform);
		m_texture->Bind(0);
		CatolYeah::Renderer::Submit(textureShader, m_squareVertexArray, imagesTransform);

		//CatolYeah::Renderer::Submit(m_triangleShader, m_triangleVertexArray);
		CatolYeah::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}
	
private:
	CatolYeah::OrthographicCamera m_camera;

	CatolYeah::Ref<CatolYeah::VertexArray> m_triangleVertexArray;
	CatolYeah::Ref<CatolYeah::Shader> m_triangleShader;

	CatolYeah::Ref<CatolYeah::VertexArray> m_squareVertexArray;
	CatolYeah::Ref<CatolYeah::Shader> m_squareShader;

	CatolYeah::ShaderLibrary m_shaderLib;
	CatolYeah::Ref<CatolYeah::Texture2D> m_texture;
	CatolYeah::Ref<CatolYeah::Texture2D> m_solidBGTexture;

	glm::vec4 m_squareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec3 m_squarePosition;
	float m_squareTranslationSpeed = 250.0f;

	glm::vec3 m_cameraPosition;
	float m_cameraTranslationSpeed = 500.0f;
	
	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 180.0f;

};

class Sandbox : public CatolYeah::Application
{
public:
	Sandbox()
	{
		CY_TRACE("Sandbox c'tor");
		PushLayer(new TestLayer());
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