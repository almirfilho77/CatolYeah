#pragma once

#include "CatolYeah/Renderer/IndexBuffer.h"

namespace CatolYeah
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_rendererId;
		uint32_t m_count;

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline unsigned int GetCount() const override { return m_count; }
	};
}