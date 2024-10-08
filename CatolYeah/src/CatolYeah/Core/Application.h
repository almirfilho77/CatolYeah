#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"
#include "CatolYeah/Core/Window.h"
#include "CatolYeah/Core/LayerStack.h"
#include "CatolYeah/Events/ApplicationEvent.h"

#include "CatolYeah/ImGui/ImGuiLayer.h"

namespace CatolYeah {

	typedef struct ApplicationSpecs
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		std::string AssetsPath;
		bool VSyncEnabled;
	};

	class Application
	{
	public:
		Application(const std::string& windowTitle = "MyApp",
			uint32_t windowWidth = 1920,
			uint32_t windowHeight = 1080,
			std::string_view assetsPath = "",
			bool setVSync = true);
		Application(const ApplicationSpecs& applicationSpecs);
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
		void m_CreateApplication(const std::string &windowTitle = "MyApp",
			uint32_t windowWidth = 1920,
			uint32_t windowHeight = 1080,
			std::string_view assetsPath = "",
			bool setVSync = true);

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