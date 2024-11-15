#include "cypch.h"
#include "Application.h"

#include "CatolYeah/Core/Assert.h"
#include "CatolYeah/Core/KeyCodes.h"
#include "CatolYeah/Core/Layer.h"
#include "CatolYeah/Core/Logger.h"
#include "CatolYeah/Core/Timestep.h"
#include "CatolYeah/Core/Window.h"
#include "CatolYeah/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace CatolYeah {
	
	Application* Application::s_instance = nullptr;

	// TODO: remove code duplication
	Application::Application(const std::string& windowTitle,
		uint32_t windowWidth,
		uint32_t windowHeight,
		std::string_view assetsPath,
		bool setVSync)
	{
		m_CreateApplication(windowTitle, windowWidth, windowHeight, assetsPath, setVSync);
	}

	Application::Application(const ApplicationSpecs& applicationSpecs)
	{
		m_CreateApplication(applicationSpecs.Title, 
			applicationSpecs.Width,
			applicationSpecs.Height,
			applicationSpecs.AssetsPath,
			applicationSpecs.VSyncEnabled);
	}

	Application::~Application()
	{
		m_running = false;
	}

	void Application::m_CreateApplication(const std::string &windowTitle, 
		uint32_t windowWidth, 
		uint32_t windowHeight, 
		std::string_view assetsPath, 
		bool setVSync)
	{
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_INFO("Creating Application");
		CY_ASSERT(s_instance == nullptr, "Application already exists");
		s_instance = this;
		WindowProps windowProps(windowTitle, windowWidth, windowHeight);
		m_window = Scope<Window>(Window::Create(windowProps));
		m_window->SetEventCallback(CY_BIND_EVENT_FN(Application::OnEvent));
		m_window->SetVSync(setVSync);
		if (!assetsPath.empty())
		{
			Renderer::Init(assetsPath);
		}
		else
		{
			Renderer::Init();
		}


		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_running = true;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(CY_BIND_EVENT_FN(Application::m_OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(CY_BIND_EVENT_FN(Application::m_OnWindowResize));

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
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_TRACE("Push regular layer into the stack");
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_TRACE("Push overlay into the stack");
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_TRACE("Application main loop");
		while (m_running)
		{
			float time = static_cast<float>(glfwGetTime());
			Timestep ts = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (m_minimized == false)
			{
				for (Layer* layer : m_layerStack) //For ranged loop possible due to begin() and end() functions defined in the class
				{
					layer->OnUpdate(ts);
				}
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
		return EVENT_RETURN_HANDLED;
	}

	bool Application::m_OnWindowResize(WindowResizeEvent& e)
	{
		CY_PROFILING_FUNCTION_TIMER();
		
		uint32_t width = e.GetWindowWidth();
		uint32_t height = e.GetWindowHeight();

		CY_CORE_INFO("Window resize event: new width [{0}] / new height [{1}]", width, height);
		if (width == 0 || height == 0)
		{
			m_minimized = true;
			return EVENT_RETURN_PASS_ON;
		}
		m_minimized = false;
		Renderer::OnWindowResize(width, height);

		return EVENT_RETURN_PASS_ON;
	}
}