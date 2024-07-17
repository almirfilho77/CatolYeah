#include "cypch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

#include "OpenGLCommon.h"

#include "CatolYeah/Core/Assert.h"

namespace CatolYeah
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t channels, uint32_t slot)
		:	m_path(""),
			m_rendererID(0),
			m_width(width), m_height(height),
			m_slot(slot),
			m_internalFormat(0), m_dataFormat(0), m_channels(channels)
	{

		switch (m_channels)
		{
		case 3:
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
			break;

		case 4:
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
			break;

		default:
			CY_ASSERT(false, "Format not supported!");
		}

		if ((m_internalFormat == 0) || (m_dataFormat == 0))
		{
			CY_ASSERT(false, "Format not supported!");
		}

		//GLCallVoid(glGenTextures(1, &m_rendererID));
		GLCallVoid(glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID));
		GLCallVoid(glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height));

		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, uint32_t slot)
		:	m_path(path),
			m_rendererID(0),
			m_width(0), m_height(0),
			m_slot(slot),
			m_internalFormat(0), m_dataFormat(0), m_channels(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			CY_ASSERT(false, "Failed to load texture image");
		}

		m_channels = channels;
		m_width = width;
		m_height = height;

		switch (m_channels)
		{
			case 3:
				m_internalFormat = GL_RGB8;
				m_dataFormat = GL_RGB;
				break;

			case 4:
				m_internalFormat = GL_RGBA8;
				m_dataFormat = GL_RGBA;
				break;

			default:
				CY_ASSERT(false, "Format not supported!");
		}

		if ((m_internalFormat == 0) || (m_dataFormat == 0))
		{
			CY_ASSERT(false, "Format not supported!");
		}

		//GLCallVoid(glGenTextures(1, &m_rendererID));
		GLCallVoid(glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID));
		GLCallVoid(glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height));

		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCallVoid(glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height,
			m_dataFormat, GL_UNSIGNED_BYTE, data));

		if(data)
		{
			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCallVoid(glDeleteTextures(1, &m_rendererID));
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		CY_ASSERT(size == m_width * m_height * m_channels, "Size of data must be equal to the entire texture");
		GLCallVoid(glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height,
			m_dataFormat, GL_UNSIGNED_BYTE, data));
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		m_slot = slot;
		GLCallVoid(glBindTextureUnit(m_slot, m_rendererID));
	}

	void OpenGLTexture2D::Unbind(uint32_t slot)
	{
		m_slot = slot;
		GLCallVoid(glBindTextureUnit(m_slot, 0));
	}

}