#include "cypch.h"
#include "Application.h"

#include "CatolYeah/Renderer/VertexBufferLayout.h"

#include "Renderer/Renderer.h"

#include "CatolYeah/KeyCodes.h"

#include "CatolYeah/Core/Timestep.h"

#include <GLFW/glfw3.h>

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
		m_window = Scope<Window>(Window::Create());
		m_window->SetEventCallback(CY_BIND_EVENT_FN(Application::OnEvent));
		m_window->SetVSync(true);
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

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
			float time = static_cast<float>(glfwGetTime());
			Timestep ts = time - m_lastFrameTime;
			m_lastFrameTime = time;

			for (Layer* layer : m_layerStack) //For ranged loop possible due to begin() and end() functions defined in the class
			{
				layer->OnUpdate(ts);
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