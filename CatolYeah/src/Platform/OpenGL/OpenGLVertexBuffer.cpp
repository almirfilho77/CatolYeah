#include "cypch.h"
#include "OpenGLVertexBuffer.h"

#include "OpenGLCommon.h"

namespace CatolYeah
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
        : m_rendererId(0)
    {
        CY_PROFILING_FUNCTION_TIMER();

        // Generate an internal buffer and assign an index to it
        GLCallVoid(glCreateBuffers(1, &m_rendererId));

        GLCallVoid(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));

        // Create the actual buffer of data, specifying at least its size
        GLCallVoid(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
        :   m_rendererId(0)
    {
        CY_PROFILING_FUNCTION_TIMER();

        // Generate an internal buffer and assign an index to it
        GLCallVoid(glCreateBuffers(1, &m_rendererId));

        GLCallVoid(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));

        // Create the actual buffer of data, specifying at least its size
        GLCallVoid(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GLCallVoid(glDeleteBuffers(1, &m_rendererId));
    }

    void OpenGLVertexBuffer::Bind() const
    {
        GLCallVoid(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GLCallVoid(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        CY_PROFILING_FUNCTION_TIMER();

        GLCallVoid(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
        GLCallVoid(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    }
}