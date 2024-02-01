#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"
#include "CatolYeah/Window.h"
#include "CatolYeah/LayerStack.h"
#include "CatolYeah/Events/ApplicationEvent.h"

#include "CatolYeah/ImGui/ImGuiLayer.h"

namespace CatolYeah {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_window; }

		inline static Application& Get() { return *s_instance; }

	private:
		bool m_OnWindowClose(WindowCloseEvent &e);
		bool m_OnWindowResize(WindowResizeEvent &e);

	private:
		static Application* s_instance;
		Scope<Window> m_window;
		bool m_running = false;
		bool m_minimized = false;

		LayerStack m_layerStack;
		ImGuiLayer *m_ImGuiLayer;

		float m_lastFrameTime = 0.0f;
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */