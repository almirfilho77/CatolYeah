#include <CatolYeah.h>
#include <iostream>

#include "imgui/imgui.h"

class DebugLayer : public CatolYeah::Layer
{
public:
	DebugLayer()
		: Layer("DebugLayer") {}

	void OnAttach() override
	{
		CY_TRACE("DebugLayer::OnAttach!");
	}

	void OnUpdate() override
	{
		
	}

	void OnImGuiRender() override
	{
		std::string gpu_info = CatolYeah::Application::Get().GetWindow().GetGPUInfo();
		ImGui::Begin("GPU Info");
		ImGui::Text(gpu_info.c_str());
		ImGui::End();
	}

	void OnEvent(CatolYeah::Event& event) override
	{
		
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

		// Triangle

		m_triangleVertexArray.reset(CatolYeah::VertexArray::Create());

		float triangle_vertices[3 * 7] = {
			 50.0f,  50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 150.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 100.0f, 150.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		std::shared_ptr<CatolYeah::VertexBuffer> triangleVB;
		triangleVB.reset(CatolYeah::VertexBuffer::Create(triangle_vertices, sizeof(triangle_vertices)));
		CatolYeah::VertexBufferLayout layout = {
			{ CatolYeah::ShaderDataType::Float3, "a_Position" },
			{ CatolYeah::ShaderDataType::Float4, "a_Color"}
		};
		triangleVB->SetBufferLayout(layout);
		m_triangleVertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<CatolYeah::IndexBuffer> triangleIB;
		triangleIB.reset(CatolYeah::IndexBuffer::Create(indices, sizeof(triangle_vertices) / sizeof(uint32_t)));
		m_triangleVertexArray->SetIndexBuffer(triangleIB);

		std::string triangle_vertex_shader = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
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

		m_triangleShader.reset(CatolYeah::Shader::Create(triangle_vertex_shader, triangle_fragment_shader));

		// Square

		m_squareVertexArray.reset(CatolYeah::VertexArray::Create());

		float square_vertices[4 * 7] = {
			25.0f,  25.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			175.0f, 25.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			175.0f, 175.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			25.0f,  175.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
		};
		std::shared_ptr<CatolYeah::VertexBuffer> squareVB;
		squareVB.reset(CatolYeah::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
		CatolYeah::VertexBufferLayout square_layout = {
			{ CatolYeah::ShaderDataType::Float3, "a_Position" },
			{ CatolYeah::ShaderDataType::Float4, "a_Color"}
		};
		squareVB->SetBufferLayout(square_layout);
		m_squareVertexArray->AddVertexBuffer(squareVB);

		uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
		std::shared_ptr<CatolYeah::IndexBuffer> squareIB;
		squareIB.reset(CatolYeah::IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t)));
		m_squareVertexArray->SetIndexBuffer(squareIB);

		std::string square_vertex_shader = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
				v_Color = a_Color;	
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

		m_squareShader.reset(CatolYeah::Shader::Create(square_vertex_shader, square_fragment_shader));

		CatolYeah::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	virtual void OnAttach() override
	{

	}

	virtual void OnUpdate() override
	{
		CatolYeah::RenderCommand::Clear();
		CatolYeah::Renderer::BeginScene(m_camera);
		CatolYeah::Renderer::Submit(m_squareShader, m_squareVertexArray);
		CatolYeah::Renderer::Submit(m_triangleShader, m_triangleVertexArray);
		CatolYeah::Renderer::EndScene();
	}

	virtual void OnEvent(CatolYeah::Event& event)
	{
		CatolYeah::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<CatolYeah::KeyPressedEvent>(CY_BIND_EVENT_FN(TestLayer::m_OnKeyPressed));
	}

private:
	bool TestLayer::m_OnKeyPressed(CatolYeah::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case CY_KEY_W:
			m_camera.SetPosition(m_camera.GetPosition() + glm::vec3(0.0f, 50.0f, 0.0f));
			break;

		case CY_KEY_S:
			m_camera.SetPosition(m_camera.GetPosition() + glm::vec3(0.0f, -50.0f, 0.0f));
			break;

		case CY_KEY_A:
			m_camera.SetPosition(m_camera.GetPosition() + glm::vec3(-50.0f, 0.0f, 0.0f));
			break;

		case CY_KEY_D:
			m_camera.SetPosition(m_camera.GetPosition() + glm::vec3(50.0f, 0.0f, 0.0f));
			break;

		case CY_KEY_E:
			m_camera.SetRotation(m_camera.GetRotation() + 5.0f);
			break;

		case CY_KEY_Q:
			m_camera.SetRotation(m_camera.GetRotation() - 5.0f);
			break;

		default:
			CY_CORE_ERROR("Not yet implemented!");
			break;
		}

		return EVENT_RETURN_PASS_ON;
	}
	
private:
	CatolYeah::OrthographicCamera m_camera;

	std::shared_ptr<CatolYeah::VertexArray> m_triangleVertexArray;
	std::shared_ptr<CatolYeah::Shader> m_triangleShader;

	std::shared_ptr<CatolYeah::VertexArray> m_squareVertexArray;
	std::shared_ptr<CatolYeah::Shader> m_squareShader;
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