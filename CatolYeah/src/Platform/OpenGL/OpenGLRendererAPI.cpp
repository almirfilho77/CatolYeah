#include "cypch.h"
#include "OpenGLRendererAPI.h"
#include "OpenGLCommon.h"

#include "CatolYeah/Core/Assert.h"

namespace CatolYeah
{
	void OpenGLRendererAPI::Init()
	{
		CY_PROFILING_FUNCTION_TIMER();

		// Enable blending: what to do with alpha channel
		GLCallVoid(glEnable(GL_BLEND));
		GLCallVoid(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Enable Depth test: what to do if a pixel is behind another
		GLCallVoid(glEnable(GL_DEPTH_TEST));
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		CY_PROFILING_FUNCTION_TIMER();

		CY_CORE_INFO("Setting the viewport here to W: {0}, H: {1}", width, height);
		GLCallVoid(glViewport(x, y, width, height));
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GLCallVoid(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::Clear()
	{
		GLCallVoid(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array)
	{
		CY_PROFILING_FUNCTION_TIMER();

		GLCallVoid(glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}