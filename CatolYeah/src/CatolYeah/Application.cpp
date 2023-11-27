#include "cypch.h"
#include "Application.h"

#include "glad/glad.h"

#include "CatolYeah/Renderer/VertexBufferLayout.h"

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

		// Triangle

		m_triangleVertexArray.reset(VertexArray::Create());

		float triangle_vertices[3 * 7] = { 
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
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
			-0.75f, -0.75f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			 0.75f, -0.75f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			 0.75f,  0.75f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.1f, 0.4f, 0.7f, 1.0f,
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
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
			glClear(GL_COLOR_BUFFER_BIT);

			m_squareShader->Bind();
			m_squareVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_squareVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			m_triangleShader->Bind();
			m_triangleVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_triangleVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

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
}