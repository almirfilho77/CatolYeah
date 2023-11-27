#pragma once

#include "CatolYeah/Renderer/VertexArray.h"
#include "CatolYeah/Renderer/VertexBuffer.h"
#include "CatolYeah/Renderer/IndexBuffer.h"

namespace CatolYeah {

	class OpenGLVertexArray : public VertexArray
	{
	private:
		unsigned int m_rendererId;
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;

		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override
		{
			return m_indexBuffer;
		}
	};
}