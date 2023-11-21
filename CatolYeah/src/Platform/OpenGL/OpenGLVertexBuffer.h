#pragma once

#include "CatolYeah/Renderer/VertexBuffer.h"

namespace CatolYeah
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_rendererId;

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	};
}