#ifndef __OPENGL_CONTEXT_H__
#define __OPENGL_CONTEXT_H__
#pragma once

#include "CatolYeah/Renderer/GraphicsContext.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG

#define GLCallVoid(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define GLCall(x) [&](){\
    GLClearError();\
    auto retval = x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))\
    return retval;\
    }()
#else

#define GlCallVoid(x) x
#define GlCall(x) x

#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

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