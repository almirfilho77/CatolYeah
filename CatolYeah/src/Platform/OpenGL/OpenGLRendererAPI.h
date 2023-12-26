#pragma once

#include "CatolYeah/Renderer/RendererAPI.h"

namespace CatolYeah
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertex_array) override;
	};
}