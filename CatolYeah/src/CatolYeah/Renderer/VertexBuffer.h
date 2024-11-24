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
		virtual void SetData(const void *data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};
}