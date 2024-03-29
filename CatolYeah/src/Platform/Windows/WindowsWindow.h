#ifndef __WINDOWS_WINDOW_H__
#define __WINDOWS_WINDOW_H__
#pragma once

#include "CatolYeah/Core/Window.h"
#include "GLFW/glfw3.h"

#include "CatolYeah/Renderer/GraphicsContext.h"

namespace CatolYeah {
	
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		bool OnUpdate() override;

		unsigned int GetWidth() const override { return m_windowData.Width; }
		unsigned int GetHeight() const override { return m_windowData.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override { return m_windowData.VSync; }

		inline virtual void* GetNativeWindow() const override { return m_window; }

		inline virtual std::string GetGPUInfo() const override { return m_context->GetGPUInfo(); }

	private:
		virtual void m_Init(const WindowProps& props);
		virtual void m_Shutdown();

	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_windowData;

	};

} //CatolYeah
#endif /* __WINDOWS_WINDOW_H__ */