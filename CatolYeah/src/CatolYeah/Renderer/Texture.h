#pragma once

namespace CatolYeah
{

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetSlot() const = 0;
		virtual uint32_t GetTextureId() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind(uint32_t slot = 0) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(uint32_t width, uint32_t height, uint32_t channels = 4, uint32_t slot = 0);
		static Ref<Texture2D> Create(const std::string& path, uint32_t slot = 0);
	};

}