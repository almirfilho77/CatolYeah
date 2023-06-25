#include "Application.h"
#include "Logger.h"

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
		while (true);
	}
}