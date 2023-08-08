#include "cypch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace CatolYeah
{
	OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
		:	m_windowHandle(window_handle)
	{
		if (window_handle == nullptr)
		{
			CY_CORE_ERROR("Window is null!");
			DEBUGBREAK
		}
	}
	
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (status == 0)
		{
			CY_CORE_ERROR("Failed to initialize Glad");
			DEBUGBREAK
		}

		const char* GPU_info = (const char*)glGetString(GL_RENDERER);
		t_GPUInfo = static_cast<std::string>(GPU_info);
		CY_CORE_INFO("GPU: {0}", t_GPUInfo);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}//CatolYeah