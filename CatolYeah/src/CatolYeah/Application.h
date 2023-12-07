#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#pragma once

#include "Core.h"
#include "CatolYeah/Window.h"
#include "CatolYeah/LayerStack.h"
#include "CatolYeah/Events/ApplicationEvent.h"

#include "CatolYeah/ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"

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
		bool m_OnKeyPressed(KeyPressedEvent& e);

	private:
		static Application* s_instance;
		std::unique_ptr<Window> m_window;
		bool m_running = false;

		LayerStack m_layerStack;
		ImGuiLayer *m_ImGuiLayer;

		OrthographicCamera m_camera;

		std::shared_ptr<VertexArray> m_triangleVertexArray;
		std::shared_ptr<Shader> m_triangleShader;

		std::shared_ptr<VertexArray> m_squareVertexArray;
		std::shared_ptr<Shader> m_squareShader;
	};

	Application* CreateApplication();

}

#endif /* __APPLICATION_H__ */