#pragma once

#include "CatolYeah/Renderer/Texture.h"

namespace CatolYeah
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();
		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }
		virtual uint32_t GetSlot() const override { return m_slot; }

		virtual void Bind(uint32_t slot = 0) override;

	private:
		std::string m_path;

		uint32_t m_rendererID;

		uint32_t m_width;
		uint32_t m_height;

		uint32_t m_slot;
	};

}