#include "cypch.h"
#include "Application.h"
#include "Logger.h"

#include "CatolYeah/Events/ApplicationEvent.h"

namespace CatolYeah {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		CY_CORE_TRACE("Application main loop");
		WindowResizeEvent evnt(1260, 1080);
		CY_CORE_DEBUG(evnt);
		while (true);
	}
}