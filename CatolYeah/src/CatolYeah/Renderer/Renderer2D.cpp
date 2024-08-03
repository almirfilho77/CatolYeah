#include "cypch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace fs = std::filesystem;

namespace CatolYeah
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> vao;
		Ref<Shader> shader;
	};

	static Renderer2DStorage* s_Data = nullptr;

	void Renderer2D::Init()
	{
		if (s_Data == nullptr)
		{
			s_Data = new Renderer2DStorage();
		}

		s_Data->vao = VertexArray::Create();

		float square_vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(square_vertices, sizeof(square_vertices));
		VertexBufferLayout square_layout = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		squareVB->SetBufferLayout(square_layout);
		s_Data->vao->AddVertexBuffer(squareVB);

		uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
		s_Data->vao->SetIndexBuffer(squareIB);

		fs::path shaderPath = fs::path("assets") / "shaders" / "SolidColor.glsl";
		s_Data->shader = Shader::Create(shaderPath.string());
	}

	// TODO: remove code duplication
	void Renderer2D::Init(std::string_view assetsPath)
	{
		if (s_Data == nullptr)
		{
			s_Data = new Renderer2DStorage();
		}

		s_Data->vao = VertexArray::Create();

		float square_vertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(square_vertices, sizeof(square_vertices));
		VertexBufferLayout square_layout = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		squareVB->SetBufferLayout(square_layout);
		s_Data->vao->AddVertexBuffer(squareVB);

		uint32_t square_indices[6] = { 0, 1, 2, 0, 2, 3 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
		s_Data->vao->SetIndexBuffer(squareIB);

		fs::path shaderPath = fs::path(assetsPath) / "SolidColor.glsl";
		s_Data->shader = Shader::Create(shaderPath.string());
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->shader->Bind();
		s_Data->shader->SetUniformMatFloat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		//s_Data->shader->SetUniformMat4f("u_ModelMatrix", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->shader->Bind();
		s_Data->shader->SetUniformFloat4("u_Color", color);
		s_Data->shader->SetUniformMatFloat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)));

		s_Data->vao->Bind();
		RenderCommand::DrawIndexed(s_Data->vao);
	}

}