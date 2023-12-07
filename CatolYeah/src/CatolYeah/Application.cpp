#include "cypch.h"
#include "Application.h"

#include "CatolYeah/Renderer/VertexBufferLayout.h"

#include "Renderer/Renderer.h"

#include "CatolYeah/KeyCodes.h"

namespace CatolYeah {
	
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance != nullptr)
		{
			CY_CORE_ERROR("Application already exists");
			DEBUGBREAK
		}
		s_instance = this;
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(CY_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Camera creation
		m_camera = OrthographicCamera(0.0f, (float)m_window->GetWidth(), 0.0f, (float)m_window->GetHeight());

		// Triangle

		m_triangleVertexArray.reset(VertexArray::Create());

		float triangle_vertices[3 * 7] = { 
			 50.0f,  50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 150.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 100.0f, 150.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};
		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(triangle_vertices, sizeof(triangle_vertices)));
		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"}
		};
		triangleVB->SetBufferLayout(layout);
		m_triangleVertexArray->AddVertexBuffer(triangleVB);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(indices, sizeof(triangle_vertices)/sizeof(uint32_t)));
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

		m_triangleShader.reset(Shader::Create(triangle_vertex_shader, triangle_fragment_shader));

		// Square

		m_squareVertexArray.reset(VertexArray::Create());

		float square_vertices[4 * 7] = {
			25.0f,  25.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			175.0f, 25.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			175.0f, 175.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			25.0f,  175.0f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
		VertexBufferLayout square_layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"}
		};
		squareVB->SetBufferLayout(square_layout);
		m_squareVertexArray->AddVertexBuffer(squareVB);

		uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(square_indices, sizeof(square_indices)/sizeof(uint32_t)));
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

		m_squareShader.reset(Shader::Create(square_vertex_shader, square_fragment_shader));

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_running = true;
	}

	Application::~Application()
	{
		m_running = false;
	}

	void Application::OnEvent(Event& e)
	{
		if (e.IsInCategory(EventCategoryKeyboard) || e.IsInCategory(EventCategoryMouseButton))
		{
			CY_CORE_DEBUG("{0}", e);
		}
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CY_BIND_EVENT_FN(Application::m_OnWindowClose));
		dispatcher.Dispatch<KeyPressedEvent>(CY_BIND_EVENT_FN(Application::m_OnKeyPressed));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
			{
				CY_CORE_DEBUG("Event sent to the layers is handled!");
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		CY_CORE_TRACE("Push regular layer into the stack");
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		CY_CORE_TRACE("Push overlay into the stack");
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		CY_CORE_TRACE("Application main loop");
		while (m_running)
		{
			RenderCommand::Clear();

			/*m_camera.SetPosition({-200.0f, -400.0f, 0});
			m_camera.SetRotation(45.0f);*/

			Renderer::BeginScene(m_camera);

			Renderer::Submit(m_squareShader, m_squareVertexArray);
			Renderer::Submit(m_triangleShader, m_triangleVertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_layerStack) //For ranged loop possible due to begin() and end() functions defined in the class
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_layerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_window->OnUpdate();
		}
		CY_CORE_TRACE("Application main loop terminated");
	}

	bool Application::m_OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::m_OnKeyPressed(KeyPressedEvent& e)
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

		return true;
	}
}