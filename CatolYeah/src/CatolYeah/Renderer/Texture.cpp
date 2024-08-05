#include "cypch.h"
#include "Texture.h"

#include "Renderer.h"
#include "RendererAPI.h"

#include "CatolYeah/Core/Assert.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace CatolYeah
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t channels, uint32_t slot)
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
            return std::make_shared<OpenGLTexture2D>(width, height, channels, slot);
        }
        }
        CY_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, uint32_t slot)
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
                return std::make_shared<OpenGLTexture2D>(path, slot);
            }
        }
        CY_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
	}

}
