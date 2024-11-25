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

		virtual std::string_view GetName() const = 0;

		virtual void SetUniformIntArray(const std::string& name, int *values, uint32_t count) = 0;

		virtual void SetUniformInt1(const std::string& name, int value) = 0;
		virtual void SetUniformFloat1(const std::string& name, float value) = 0;

		virtual void SetUniformFloat3(const std::string& name, float v0, float v1, float v2) = 0;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vector) = 0;

		virtual void SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3) = 0;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vector) = 0;

		virtual void SetUniformMatFloat4(const std::string& name, const glm::mat4& matrix) = 0;

		static Ref<Shader> Create(std::string_view filepath);
		static Ref<Shader> Create(const std::string &name, const std::string& vertex_src, const std::string& fragment_src);
	};

	class ShaderLibrary
	{
	public:
		void Add(std::string_view name, Ref<Shader> shader);
		void Add(Ref<Shader> shader);
		Ref<Shader> Load(const std::string& name, std::string_view filepath);
		Ref<Shader> Load(std::string_view filepath);
		Ref<Shader> Get(std::string_view name);

	private:
		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
}