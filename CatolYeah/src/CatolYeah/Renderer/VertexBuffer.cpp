#include "cypch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace CatolYeah
{
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            {
                CY_CORE_ERROR("RendererAPI::None is currently not supported!");
                DEBUGBREAK
                return nullptr;
            }
        
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
    }
}