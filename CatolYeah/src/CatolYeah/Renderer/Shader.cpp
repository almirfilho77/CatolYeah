#include "cypch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace CatolYeah
{
	Ref<Shader> Shader::Create(std::string_view filepath)
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
                return std::make_shared<OpenGLShader>(filepath);
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
	}

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
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
                return std::make_shared<OpenGLShader>(name, vertex_src, fragment_src);
            }
        }
        CY_CORE_ERROR("Unkown RendererAPI!");
        DEBUGBREAK
        return nullptr;
	}

    void ShaderLibrary::Add(std::string_view name, Ref<Shader> shader)
    {
        std::string shader_name = name.data();
        if (Exists(shader_name))
        {
            CY_CORE_ERROR("Shader already exists");
            DEBUGBREAK
        }
        m_shaders[shader_name] = shader;
    }

    void ShaderLibrary::Add(Ref<Shader> shader)
    {
        std::string name = shader->GetName().data();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, std::string_view filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(std::string_view filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(std::string_view name)
    {
        std::string shader_name = name.data();
        if (!Exists(shader_name))
        {
            CY_CORE_ERROR("Shader does not exist");
            DEBUGBREAK
        }
        return m_shaders[shader_name];
    }

    bool ShaderLibrary::Exists(const std::string &name)
    {
        return m_shaders.find(name) != m_shaders.end();
    }
}