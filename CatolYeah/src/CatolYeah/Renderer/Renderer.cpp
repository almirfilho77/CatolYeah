#include "cypch.h"
#include "Renderer.h"

namespace CatolYeah
{
	glm::mat4 Renderer::m_viewProjectionMatrix;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera &camera)
	{
		Renderer::m_viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4& model_matrix)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjectionMatrix", Renderer::m_viewProjectionMatrix);
		shader->SetUniformMat4f("u_ModelMatrix", model_matrix);

		vertex_array->Bind();
		RenderCommand::DrawIndexed(vertex_array);
	}
}