#pragma once

#include "CatolYeah/Renderer/Texture.h"

#include "glad/glad.h"

namespace CatolYeah
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t channels = 4, uint32_t slot = 0);
		OpenGLTexture2D(const std::string& path, uint32_t slot = 0);
		virtual ~OpenGLTexture2D();
		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual uint32_t GetSlot() const override { return m_slot; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) override;
		virtual void Unbind(uint32_t slot) override;

	private:
		std::string m_path;

		uint32_t m_rendererID;

		uint32_t m_width;
		uint32_t m_height;

		uint32_t m_slot;

		GLenum m_internalFormat, m_dataFormat;
		uint32_t m_channels;
	};

}