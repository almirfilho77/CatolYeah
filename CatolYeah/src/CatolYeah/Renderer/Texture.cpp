#include "cypch.h"
#include "Texture.h"

#include "Renderer.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace CatolYeah
{

	Ref<Texture2D> Texture2D::Create(const std::string& path)
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
                return std::make_shared<OpenGLTexture2D>(path);
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
	}

}
