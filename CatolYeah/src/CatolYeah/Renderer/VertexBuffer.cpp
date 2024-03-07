#include "cypch.h"
#include "VertexBuffer.h"

#include "Renderer.h"
#include "CatolYeah/Core/Assert.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace CatolYeah
{
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            {
                CY_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            }
        
            case RendererAPI::API::OpenGL:
            {
                return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            }
        }
        CY_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
    }
}