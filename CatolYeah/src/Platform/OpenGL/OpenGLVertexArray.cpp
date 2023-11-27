#include "cypch.h"
#include "OpenGLVertexArray.h"

#include "OpenGLCommon.h"

#include "CatolYeah/Renderer/VertexBufferLayout.h"

namespace CatolYeah
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		CY_CORE_ERROR("Unknown ShaderDataType");
		DEBUGBREAK
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GLCallVoid(glCreateVertexArrays(1, &m_rendererId));
		GLCallVoid(glBindVertexArray(m_rendererId));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLCallVoid(glDeleteVertexArrays(1, &m_rendererId));
	}

	void OpenGLVertexArray::Bind() const
	{
		GLCallVoid(glBindVertexArray(m_rendererId));
	}

	void OpenGLVertexArray::Unbind() const
	{
		GLCallVoid(glBindVertexArray(0));
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer)
	{
		if (vertex_buffer->GetBufferLayout().GetElements().size() == 0)
		{
			CY_CORE_ERROR("There is no buffer layout associated to the vertex buffer");
			DEBUGBREAK
		}

		GLCallVoid(glBindVertexArray(m_rendererId));
		vertex_buffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertex_buffer->GetBufferLayout();
		for (const auto& element : layout)
		{
			GLCallVoid(glEnableVertexAttribArray(index));
			GLCallVoid(glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset));
			index++;
		}

		m_vertexBuffers.push_back(vertex_buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer)
	{
		GLCallVoid(glBindVertexArray(m_rendererId));
		index_buffer->Bind();
		m_indexBuffer = index_buffer;
	}
}