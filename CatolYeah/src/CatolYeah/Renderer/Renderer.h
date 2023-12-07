#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace CatolYeah
{
	class Renderer
	{
	public:
		// Functions to create the environment around the scene (camera, lights, etc)
		static void BeginScene(const OrthographicCamera &camera);
		static void EndScene();

		// Function to submit the meshes loaded inside the Vertex Array to the draw call
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertex_array);

		// Helper function to get the current used Graphics API
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static glm::mat4 m_viewProjectionMatrix;
	};
}