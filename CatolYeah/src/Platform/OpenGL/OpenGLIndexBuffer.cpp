#include "cypch.h"
#include "OpenGLIndexBuffer.h"

#include "OpenGLCommon.h"

namespace CatolYeah
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        :   m_rendererId(0),
            m_count(count)
    {
        ASSERT(sizeof(uint32_t) == sizeof(GLuint));

        // Generate an internal buffer and assign an index to it
        GLCallVoid(glCreateBuffers(1, &m_rendererId));

        GLCallVoid(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));

        // Create the actual buffer of data, specifying at least its size
        GLCallVoid(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        GLCallVoid(glDeleteBuffers(1, &m_rendererId));
    }

    void OpenGLIndexBuffer::Bind() const
    {
        GLCallVoid(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        GLCallVoid(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }
}