#include "cypch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace CatolYeah
{
	glm::mat4 Renderer::m_viewProjectionMatrix;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	// TODO: remove code duplication
	void Renderer::Init(std::string_view assetsPath)
	{
		RenderCommand::Init();
		Renderer2D::Init(assetsPath);
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
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
		shader->SetUniformMatFloat4("u_ViewProjectionMatrix", Renderer::m_viewProjectionMatrix);
		shader->SetUniformMatFloat4("u_ModelMatrix", model_matrix);

		vertex_array->Bind();
		RenderCommand::DrawIndexed(vertex_array);
	}
}