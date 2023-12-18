#pragma once

#include "CatolYeah/Renderer/VertexArray.h"
#include "CatolYeah/Renderer/VertexBuffer.h"
#include "CatolYeah/Renderer/IndexBuffer.h"

namespace CatolYeah {

	class OpenGLVertexArray : public VertexArray
	{
	private:
		unsigned int m_rendererId;
		std::vector<Ref<VertexBuffer>> m_vertexBuffers;
		Ref<IndexBuffer> m_indexBuffer;

	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const override;
		void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override
		{
			return m_indexBuffer;
		}
	};
}