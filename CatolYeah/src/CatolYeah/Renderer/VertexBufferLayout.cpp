#include "cypch.h"
#include "VertexBufferLayout.h"

namespace CatolYeah
{
	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements)
		: m_stride(0),
		m_elements(elements)
	{
		m_CalculateOffsetAndStride();
	}

	void VertexBufferLayout::m_CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.Offset += offset;
			offset += element.Size;
			m_stride += element.Size;
		}
	}
}