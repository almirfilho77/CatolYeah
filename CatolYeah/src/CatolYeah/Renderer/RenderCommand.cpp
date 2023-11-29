#include "cypch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CatolYeah
{
	// Resolve the current used Graphics API in runtime
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}