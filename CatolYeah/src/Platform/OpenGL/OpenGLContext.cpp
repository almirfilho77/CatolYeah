#include "cypch.h"
#include "OpenGLContext.h"
#include "OpenGLCommon.h"

#include "CatolYeah/Core/Assert.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace CatolYeah
{
	OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
		:	m_windowHandle(window_handle)
	{
		CY_ASSERT(window_handle != nullptr, "Window is null!");
	}
	
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CY_ASSERT(status != 0, "Failed to initialize Glad");
		
		const char* GPU_info = (const char*)glGetString(GL_RENDERER);
		t_GPUInfo = static_cast<std::string>(GPU_info);
		CY_CORE_INFO("GPU: {0}", t_GPUInfo);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}//CatolYeah