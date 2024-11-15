#include "cypch.h"
#include "WindowsWindow.h"

#include "CatolYeah/Core/Assert.h"

#include "CatolYeah/Events/ApplicationEvent.h"
#include "CatolYeah/Events/KeyEvent.h"
#include "CatolYeah/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace CatolYeah
{
	static bool s_ISGLFWInitialized = false;

	static void s_GlfwErrorCallback(int error_code, const char* description)
	{
		CY_CORE_ERROR("GLFW Error received!\nERROR CODE: {0}\nDESCRIPTION: {1}", error_code, description);
	}
	
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_TRACE("WindowsWindow c'tor");
		m_Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_TRACE("WindowsWindow d'tor");
		m_Shutdown();
	}

	bool WindowsWindow::OnUpdate()
	{
		CY_PROFILING_FUNCTION_TIMER();

		glfwPollEvents();
		m_context->SwapBuffers();
		return true;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback) 
	{
		m_windowData.EventCallback = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		CY_PROFILING_FUNCTION_TIMER();

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
		CY_PROFILING_FUNCTION_TIMER();

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
				CY_ASSERT(false, "Could not initialize GLFW!");
			}
			glfwSetErrorCallback(s_GlfwErrorCallback);
			s_ISGLFWInitialized = true;
		}

		m_window = glfwCreateWindow(m_windowData.Width, m_windowData.Height, m_windowData.Title.c_str(), nullptr, nullptr);

		m_context = new OpenGLContext(m_window);
		m_context->Init();
		
		glfwSetWindowUserPointer(m_window, &m_windowData);
		SetVSync(true);

		// Set GLFWWindow callbacks
		glfwSetWindowSizeCallback(m_window, 
			[](GLFWwindow* window, int width, int height) 
			{
				auto window_data_ptr = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
				window_data_ptr->Width = width;
				window_data_ptr->Height = height;
				window_data_ptr->EventCallback(WindowResizeEvent(width, height));
			}
		);

		glfwSetWindowCloseCallback(m_window,
			[](GLFWwindow* window)
			{
				auto window_data_ptr = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				window_data_ptr->EventCallback(WindowCloseEvent());
			}
		);

		glfwSetKeyCallback(m_window, 
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto window_data_ptr = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						window_data_ptr->EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						window_data_ptr->EventCallback(event);
						break;
					}

					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						window_data_ptr->EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetCharCallback(m_window,
			[](GLFWwindow* window, unsigned int codepoint)
			{
				auto window_data_ptr = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				window_data_ptr->EventCallback(KeyTypedEvent(codepoint));
			}
		);

		glfwSetMouseButtonCallback(m_window, 
			[](GLFWwindow* window, int button, int action, int mods)
			{
				auto window_data_ptr = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						window_data_ptr->EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						window_data_ptr->EventCallback(event);
						break;
					}
				}
			}
		);

		glfwSetScrollCallback(m_window, 
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				auto window_data_ptr = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				window_data_ptr->EventCallback(MouseScrolledEvent(static_cast<float>(xoffset), static_cast<float>(yoffset)));
			}
		);

		glfwSetCursorPosCallback(m_window,
			[](GLFWwindow* window, double xpos, double ypos)
			{
				auto window_data_ptr = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				window_data_ptr->EventCallback(MouseMovedEvent(static_cast<float>(xpos), static_cast<float>(ypos)));
			}
		);
	}

	void WindowsWindow::m_Shutdown()
	{
		glfwDestroyWindow(m_window);
	}

}//CatolYeah