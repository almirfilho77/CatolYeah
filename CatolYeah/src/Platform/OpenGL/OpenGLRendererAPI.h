#pragma once

#include "CatolYeah/Renderer/RendererAPI.h"

namespace CatolYeah
{
	class OpenGLRendererAPI : public RendererAPI
	{
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) override;
	};
}