#include "cypch.h"
#include "OpenGLCommon.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		CY_CORE_ERROR("OpenGL Error {0} > {1} [{2}:{3}]", error, function, file, line);
		return false;
	}
	return true;
}