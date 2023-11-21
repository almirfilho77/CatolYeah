#pragma once

namespace CatolYeah
{
	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& vertex_src, const std::string& fragment_src);
	};
}