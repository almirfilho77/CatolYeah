#pragma once

#include "RendererAPI.h"

namespace CatolYeah
{

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_rendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertex_array)
		{
			s_rendererAPI->DrawIndexed(vertex_array);
		}

	private:
		static RendererAPI *s_rendererAPI;	// Holds the interface pointer to the RendererAPI specific implementation
	};

}