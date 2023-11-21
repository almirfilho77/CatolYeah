#include "cypch.h"
#include "OpenGLVertexBuffer.h"

#include "OpenGLCommon.h"

namespace CatolYeah
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
        :   m_rendererId(0)
    {
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
}