#include "cypch.h"
#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace fs = std::filesystem;

#define MAXQUADSPERDRAWCALL 10000
#define VERTICESPERQUAD		4
#define INDICESPERQUAD		6

namespace CatolYeah
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoord;
		float TextureIndex;			// Specifies the sampler2D in the shader
		float TilingFactor;
	};

	struct Renderer2DStorage
	{
		const uint32_t MaxQuadsPerDrawCall = MAXQUADSPERDRAWCALL;
		const uint32_t MaxVerticesPerDrawCall = MaxQuadsPerDrawCall * VERTICESPERQUAD;
		const uint32_t MaxIndicesPerDrawCall = MaxQuadsPerDrawCall * INDICESPERQUAD;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> vao;
		Ref<VertexBuffer> quadVBO;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // Slot 0 is reserved for White Texture

		glm::vec4 QuadVertexPositions[4];
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::m_InitializeBuffers()
	{
		CY_PROFILING_FUNCTION_TIMER();

		s_Data.vao = VertexArray::Create();

		/*float square_vertices[4 * 5] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,	1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f,	0.0f, 1.0f,
		};*/

		s_Data.quadVBO = VertexBuffer::Create(s_Data.MaxVerticesPerDrawCall * sizeof(QuadVertex)); // For now, 40000 * 5 = 200000
		VertexBufferLayout quadVBLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }, 
			{ ShaderDataType::Float2, "a_TextureCoord" },
			{ ShaderDataType::Float, "a_TextureIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		};
		s_Data.quadVBO->SetBufferLayout(quadVBLayout);
		s_Data.vao->AddVertexBuffer(s_Data.quadVBO);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVerticesPerDrawCall];

		uint32_t *quadIndices = new uint32_t[s_Data.MaxIndicesPerDrawCall];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndicesPerDrawCall; i += 6)
		{
			quadIndices[i + 0] = 0 + offset;
			quadIndices[i + 1] = 1 + offset;
			quadIndices[i + 2] = 2 + offset;
			quadIndices[i + 3] = 2 + offset;
			quadIndices[i + 4] = 0 + offset;
			quadIndices[i + 5] = 3 + offset;
			offset += 4;
		}

		Ref<IndexBuffer> quadIB;
		quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndicesPerDrawCall);
		s_Data.vao->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		int samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
		{
			samplers[i] = i;
		}
		s_Data.textureShader->Bind();
		s_Data.textureShader->SetUniformIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.whiteTexture = Texture2D::Create(1, 1, 4);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = s_Data.whiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Init()
	{
		CY_PROFILING_FUNCTION_TIMER();

		fs::path shaderPath = fs::path("assets") / "shaders" / "Texture.glsl";
		s_Data.textureShader = Shader::Create(shaderPath.string());

		m_InitializeBuffers();

	}

	// TODO: remove code duplication
	void Renderer2D::Init(std::string_view assetsPath)
	{
		CY_PROFILING_FUNCTION_TIMER();

        fs::path shaderPath = fs::path(assetsPath) / "Texture.glsl";
		s_Data.textureShader = Shader::Create(shaderPath.string());
        
		m_InitializeBuffers();
	}

	void Renderer2D::Shutdown()
	{
		
	}

	// TODO: Evaluate the necessity of this call if no cameras are used
	void Renderer2D::BeginScene()
	{
		CY_PROFILING_FUNCTION_TIMER();

		s_Data.textureShader->Bind();
		s_Data.textureShader->SetUniformMatFloat4("u_ViewProjectionMatrix", glm::mat4(1.0f));
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		CY_PROFILING_FUNCTION_TIMER();

		s_Data.textureShader->Bind();
		s_Data.textureShader->SetUniformMatFloat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		//s_Data.shader->SetUniformMat4f("u_ModelMatrix", glm::mat4(1.0f));

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		CY_PROFILING_FUNCTION_TIMER();

		// Upload VBO to the GPU
		//uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase; //TheCherno way
		uint32_t dataSize = 4 * (uint32_t)sizeof(QuadVertex) * (uint32_t)((float)(s_Data.QuadIndexCount) / (float)INDICESPERQUAD); //My way
		s_Data.quadVBO->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.vao, s_Data.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		CY_PROFILING_FUNCTION_TIMER();

		const float textureIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;
		
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D> texture, const glm::vec4& color, float tiling_factor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color, tiling_factor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, const glm::vec4& color, float tiling_factor)
	{
		CY_PROFILING_FUNCTION_TIMER();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->GetTextureId() == texture->GetTextureId())
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationInRad, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationInRad, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationInRad, const glm::vec4& color)
	{
		CY_PROFILING_FUNCTION_TIMER();

		float textureIndex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotationInRad, glm::vec3({ 0.0f, 0.0f, 1.0f }))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotationInRad, Ref<Texture2D> texture, const glm::vec4& color, float tiling_factor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotationInRad, texture, color, tiling_factor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotationInRad, Ref<Texture2D> texture, const glm::vec4& color, float tiling_factor)
	{
		CY_PROFILING_FUNCTION_TIMER();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i]->GetTextureId() == texture->GetTextureId())
			{
				textureIndex = static_cast<float>(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotationInRad, glm::vec3({ 0.0f, 0.0f, 1.0f }))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tiling_factor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;		
	}

}