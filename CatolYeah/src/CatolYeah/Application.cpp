#include "cypch.h"
#include "Application.h"
#include "Logger.h"

#include "CatolYeah/Events/ApplicationEvent.h"

namespace CatolYeah {

	Application::Application()
	{
		m_window = std::unique_ptr<Window>(Window::Create());
		m_running = true;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		CY_CORE_TRACE("Application main loop");
		while (m_running)
		{
			m_window->OnUpdate();
		}
	}
}