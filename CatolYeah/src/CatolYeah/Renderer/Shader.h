#pragma once

#include <glm/glm.hpp>

namespace CatolYeah
{
	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1i(const std::string& name, int value) = 0;

		virtual void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& vector) = 0;

		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) = 0;

		static Shader* Create(const std::string& filepath);
		static Shader* Create(const std::string& vertex_src, const std::string& fragment_src);
	};
}