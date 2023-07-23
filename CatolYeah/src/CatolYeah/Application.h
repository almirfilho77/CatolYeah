#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"
#include "CatolYeah/Window.h"
#include "CatolYeah/LayerStack.h"
#include "CatolYeah/Events/ApplicationEvent.h"

namespace CatolYeah {

	class CATOLYEAH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool m_OnWindowClose(WindowCloseEvent &e);
	private:
		std::unique_ptr<Window> m_window;
		bool m_running = false;
		LayerStack m_layerStack;
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */