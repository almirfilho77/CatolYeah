#ifndef __WINDOWS_WINDOW_H__
#define __WINDOWS_WINDOW_H__
#pragma once

#include "CatolYeah/Window.h"
#include "GLFW/glfw3.h"

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

	private:
		virtual void m_Init(const WindowProps& props);
		virtual void m_Shutdown();

	private:
		GLFWwindow* m_window;

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