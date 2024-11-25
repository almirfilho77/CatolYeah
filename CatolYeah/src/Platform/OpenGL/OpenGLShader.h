#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "glad/glad.h"
#include "CatolYeah/Renderer/Shader.h"

namespace CatolYeah {

	struct ShaderSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string_view filepath);
		OpenGLShader(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual std::string_view GetName() const override { return m_name; }

		virtual void SetUniformIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual void SetUniformInt1(const std::string& name, int value) override;
		virtual void SetUniformFloat1(const std::string& name, float value) override;

		virtual void SetUniformFloat3(const std::string& name, float v0, float v1, float v2) override;
		virtual void SetUniformFloat3(const std::string& name, const glm::vec3& vector) override;

		virtual void SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3) override;
		virtual void SetUniformFloat4(const std::string& name, const glm::vec4& vector) override;
		
		virtual void SetUniformMatFloat4(const std::string& name, const glm::mat4& matrix) override;
	
	private:
		ShaderSource m_ParseShaderSource(const std::string& filepath);	// Think about deprecating this
		void m_CreateShaderSourceMap(std::string_view filepath);
		unsigned int m_CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
		unsigned int m_CreateShader(const std::unordered_map<GLenum, std::string>& shaderSources);
		unsigned int m_CompileShader(unsigned int type, const std::string& source);

		int m_GetUniformLocation(const std::string& name);
	
	private:
		unsigned int m_rendererId;
		std::string m_name;
		std::unordered_map<GLenum, std::string> m_shaderSourceMap;
		std::unordered_map<std::string, int> m_uniformLocationMap;
	};

}//CatolYeah