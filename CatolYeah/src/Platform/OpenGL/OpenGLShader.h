#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "CatolYeah/Renderer/Shader.h"

namespace CatolYeah {

	struct ShaderSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class OpenGLShader : public Shader
	{
	private:
		unsigned int m_rendererId;
		std::unordered_map<std::string, int> m_uniformLocationMap;

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertex_src, const std::string& fragment_src);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		ShaderSource m_ParseShaderSource(const std::string& filpath);
		unsigned int m_CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
		unsigned int m_CompileShader(unsigned int type, const std::string& source);

		int m_GetUniformLocation(const std::string& name);
	};

}//CatolYeah