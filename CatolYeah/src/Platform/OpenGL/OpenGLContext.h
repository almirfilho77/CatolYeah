#ifndef __OPENGL_CONTEXT_H__
#define __OPENGL_CONTEXT_H__
#pragma once

#include "CatolYeah/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace CatolYeah
{

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window_handle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;
	};

}//CatolYeah

#endif /* __OPENGL_CONTEXT_H__ */