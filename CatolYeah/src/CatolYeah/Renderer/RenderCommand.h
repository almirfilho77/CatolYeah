#pragma once

#include "RendererAPI.h"

namespace CatolYeah
{

	class RenderCommand
	{
	public:
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