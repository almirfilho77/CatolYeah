#include "cypch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace CatolYeah
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
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
                return new OpenGLVertexBuffer(vertices, size);
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
    }
}