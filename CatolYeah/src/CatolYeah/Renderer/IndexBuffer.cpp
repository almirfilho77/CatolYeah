#include "cypch.h"
#include "IndexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace CatolYeah
{
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
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
                return new OpenGLIndexBuffer(indices, count);
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
	}
}