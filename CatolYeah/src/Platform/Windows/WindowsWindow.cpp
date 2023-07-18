#include "cypch.h"
#include "WindowsWindow.h"

#include "CatolYeah/Logger.h"

namespace CatolYeah
{
	static bool s_ISGLFWInitialized = false;
	
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		CY_CORE_TRACE("WindowsWindow c'tor");
		m_Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		CY_CORE_TRACE("WindowsWindow d'tor");
		m_Shutdown();
	}

	bool WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
		return true;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback) 
	{

	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_windowData.VSync = enabled;
	}

	void WindowsWindow::m_Init(const WindowProps& props)
	{
		m_windowData.Title = props.Title;
		m_windowData.Width = props.Width;
		m_windowData.Height = props.Height;

		CY_CORE_INFO("Creating window {0} with W={1} and H={2}", m_windowData.Title,
																 m_windowData.Width,
																 m_windowData.Height);

		if (!s_ISGLFWInitialized)
		{
			CY_CORE_DEBUG("Initializing GLFW");
			int success = glfwInit();
			if (success == GLFW_FALSE)
			{
				CY_CORE_ERROR("Could not initialize GLFW!");
				DEBUGBREAK
			}
			s_ISGLFWInitialized = true;
		}

		m_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_windowData);
		SetVSync(true);
	}

	void WindowsWindow::m_Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

}//CatolYeah