#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace CatolYeah
{
	class Renderer
	{
	public:
		// Initialization code for the specifc RendererAPI
		static void Init();
		static void Init(std::string_view assetsPath);
		static void OnWindowResize(uint32_t width, uint32_t height);

		// Functions to create the environment around the scene (camera, lights, etc)
		static void BeginScene(const OrthographicCamera &camera);
		static void EndScene();

		// Function to submit the meshes loaded inside the Vertex Array to the draw call
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertex_array, const glm::mat4 &model_matrix = glm::mat4(1.0f));

		// Helper function to get the current used Graphics API
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static glm::mat4 m_viewProjectionMatrix;
	};
}