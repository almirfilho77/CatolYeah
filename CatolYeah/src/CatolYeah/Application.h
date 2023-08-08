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
	private:
		static Application* s_instance;
		std::unique_ptr<Window> m_window;
		bool m_running = false;

		LayerStack m_layerStack;
		ImGuiLayer *m_ImGuiLayer;

		unsigned int m_vertexArray, m_vertexBuffer, m_indexBuffer;
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */