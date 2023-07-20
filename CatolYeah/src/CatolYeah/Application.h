#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"
#include "CatolYeah/Window.h"
#include "CatolYeah/Events/ApplicationEvent.h"
#include "CatolYeah/Events/KeyEvent.h"
#include "CatolYeah/Events/MouseEvent.h"

namespace CatolYeah {

	class CATOLYEAH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool m_OnWindowClose(WindowCloseEvent &e);
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = false;
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */