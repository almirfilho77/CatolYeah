#include "cypch.h"
#include "Application.h"
#include "Logger.h"

namespace CatolYeah {

#define BIND_EVENT_FN(x)	std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		m_running = true;
	}

	Application::~Application()
	{
		m_running = false;
	}

	void Application::OnEvent(Event& e)
	{
		CY_CORE_DEBUG("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(m_OnWindowClose));
	}

	void Application::Run()
	{
		CY_CORE_TRACE("Application main loop");
		while (m_running)
		{
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