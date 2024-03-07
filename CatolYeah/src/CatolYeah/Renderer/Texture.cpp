#include "cypch.h"
#include "Texture.h"

#include "Renderer.h"
#include "RendererAPI.h"

#include "CatolYeah/Core/Assert.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace CatolYeah
{

	Ref<Texture2D> Texture2D::Create(const std::string& path)
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
                return std::make_shared<OpenGLTexture2D>(path);
            }
        }
        CY_ASSERT(false, "Unkown RendererAPI!");
        return nullptr;
	}

}
