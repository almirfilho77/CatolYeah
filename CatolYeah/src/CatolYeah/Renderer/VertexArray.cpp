#include "cypch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CatolYeah
{
	VertexArray* VertexArray::Create()
	{
        switch (Renderer::GetRendererAPI())
        {
            case RendererAPI::None:
            {
                CY_CORE_ERROR("RendererAPI::None is currently not supported!");
                DEBUGBREAK
                    return nullptr;
            }

            case RendererAPI::OpenGL:
            {
                return new OpenGLVertexArray();
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
	}
}