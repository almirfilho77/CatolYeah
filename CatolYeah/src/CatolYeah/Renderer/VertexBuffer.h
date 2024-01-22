#pragma once

#include "VertexBufferLayout.h"

namespace CatolYeah
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const VertexBufferLayout& GetBufferLayout() const = 0;
		virtual void SetBufferLayout(const VertexBufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};
}