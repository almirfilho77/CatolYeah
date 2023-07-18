#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"
#include "Window.h"

namespace CatolYeah {

	class CATOLYEAH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = false;
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */