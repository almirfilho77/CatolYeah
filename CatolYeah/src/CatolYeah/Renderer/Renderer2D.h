#pragma once

#include "Camera.h"
#include "Texture.h"

namespace CatolYeah
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Init(std::string_view assetsPath);
		static void Shutdown();

		static void BeginScene();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D> texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float tiling_factor = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float tiling_factor = 1.0f);

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture2D> texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float tiling_factor = 1.0f);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture2D> texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float tiling_factor = 1.0f);

	private:
		static void m_InitializeBuffers();
	};

}