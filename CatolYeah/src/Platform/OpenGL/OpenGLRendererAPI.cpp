#include "cypch.h"
#include "OpenGLRendererAPI.h"

#include "OpenGLCommon.h"

namespace CatolYeah
{
	void OpenGLRendererAPI::Init()
	{
		GLCallVoid(glEnable(GL_BLEND));
		GLCallVoid(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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
		GLCallVoid(glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}