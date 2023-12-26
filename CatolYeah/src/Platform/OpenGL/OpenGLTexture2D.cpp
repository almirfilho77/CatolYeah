#include "cypch.h"
#include "OpenGLTexture2D.h"

#include "glad/glad.h"
#include "stb_image.h"

#include "OpenGLCommon.h"

namespace CatolYeah
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:	m_path(path),
			m_rendererID(0),
			m_width(0), m_height(0),
			m_slot(0)
	{
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data == nullptr)
		{
			CY_CORE_ERROR("Failed to load texture image");
			DEBUGBREAK
		}

		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		switch (channels)
		{
			case 3:
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
				break;

			case 4:
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
				break;

			default:
				CY_CORE_ERROR("Format not supported!");
				DEBUGBREAK
		}

		if ((internalFormat == 0) || (dataFormat == 0))
		{
			CY_CORE_ERROR("Format not supported!");
			DEBUGBREAK
		}

		//GLCallVoid(glGenTextures(1, &m_rendererID));
		GLCallVoid(glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID));
		GLCallVoid(glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height));

		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCallVoid(glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCallVoid(glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height,
			dataFormat, GL_UNSIGNED_BYTE, data));

		if(data)
		{
			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCallVoid(glDeleteTextures(1, &m_rendererID));
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		m_slot = slot;
		GLCallVoid(glBindTextureUnit(m_slot, m_rendererID));
	}

}