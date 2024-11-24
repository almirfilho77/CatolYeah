#pragma once

#include "CatolYeah/Renderer/VertexBuffer.h"
#include "CatolYeah/Renderer/VertexBufferLayout.h"

namespace CatolYeah
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_rendererId;
		VertexBufferLayout m_bufferLayout;

	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const VertexBufferLayout& GetBufferLayout() const override { return m_bufferLayout; }
		virtual void SetBufferLayout(const VertexBufferLayout& layout) override { m_bufferLayout = layout; }
		virtual void SetData(const void *data, uint32_t size) override;
	};
}