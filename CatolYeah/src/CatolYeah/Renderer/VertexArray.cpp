#include "cypch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "CatolYeah/Core/Assert.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace CatolYeah
{
	Ref<VertexArray> VertexArray::Create()
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
                return std::make_shared<OpenGLVertexArray>();
            }
        }
        CY_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
	}
}