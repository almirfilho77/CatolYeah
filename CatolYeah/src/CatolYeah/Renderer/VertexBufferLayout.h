#pragma once

namespace CatolYeah 
{
	
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return sizeof(float);
			case ShaderDataType::Float2:	return sizeof(float) * 2;
			case ShaderDataType::Float3:	return sizeof(float) * 3;
			case ShaderDataType::Float4:	return sizeof(float) * 4;
			case ShaderDataType::Mat3:		return sizeof(float) * 3 * 3;
			case ShaderDataType::Mat4:		return sizeof(float) * 4 * 4;
			case ShaderDataType::Int:		return sizeof(int);
			case ShaderDataType::Int2:		return sizeof(int) * 2;
			case ShaderDataType::Int3:		return sizeof(int) * 3;
			case ShaderDataType::Int4:		return sizeof(int) * 4;
			case ShaderDataType::Bool:		return sizeof(bool);
		}
		CY_CORE_ERROR("Unknown ShaderDataType");
		DEBUGBREAK
		return 0;
	}

	struct VertexBufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		bool Normalized;
		uint32_t Offset;

		VertexBufferElement() {}

		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized), Offset(0)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}
			CY_CORE_ERROR("Unknown ShaderDataType");
			DEBUGBREAK
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() {}
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);

		inline uint32_t GetStride() const { return m_stride; }
		inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }

		std::vector<VertexBufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		void m_CalculateOffsetAndStride();

	private:
		uint32_t m_stride;
		std::vector<VertexBufferElement> m_elements;
	};

}