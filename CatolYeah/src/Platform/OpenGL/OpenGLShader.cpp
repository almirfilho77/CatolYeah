#include "cypch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/glad.h"

#include "OpenGLCommon.h"

#include <glm/gtc/type_ptr.hpp>

namespace CatolYeah {
    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        ShaderSource shader_source = m_ParseShaderSource(filepath);

        CY_CORE_DEBUG("VERTEX SHADER:");
        CY_CORE_DEBUG("{0}", shader_source.VertexSource);
        CY_CORE_DEBUG("FRAGMENT SHADER:");
        CY_CORE_DEBUG("{0}", shader_source.FragmentSource);

        m_rendererId = m_CreateShader(shader_source.VertexSource, shader_source.FragmentSource);
        Bind();
    }

    OpenGLShader::OpenGLShader(const std::string& vertex_src, const std::string& fragment_src)
    {
        CY_CORE_DEBUG("VERTEX SHADER:");
        CY_CORE_DEBUG("{0}", vertex_src);
        CY_CORE_DEBUG("FRAGMENT SHADER:");
        CY_CORE_DEBUG("{0}", fragment_src);

        m_rendererId = m_CreateShader(vertex_src, fragment_src);
        Bind();
    }

    OpenGLShader::~OpenGLShader()
    {
        GLCallVoid(glDeleteProgram(m_rendererId));
    }

    void OpenGLShader::Bind() const
    {
        GLCallVoid(glUseProgram(m_rendererId));
    }

    void OpenGLShader::Unbind() const
    {
        GLCallVoid(glUseProgram(m_rendererId));
    }

    ShaderSource OpenGLShader::m_ParseShaderSource(const std::string& filepath)
    {
        std::fstream shader_file_stream(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1,
        };

        std::stringstream ss[2];
        ShaderType shader_type = ShaderType::NONE;

        std::string line;
        while (getline(shader_file_stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                // If we turn the following behavior into a function we can write unit tests 
                // to verify if the shader type is being correctly set
                if (line.find("vertex") != std::string::npos)
                {
                    shader_type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    shader_type = ShaderType::FRAGMENT;
                }
            }
            else if (shader_type != ShaderType::NONE)
            {
                ss[(int)shader_type] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    unsigned int OpenGLShader::m_CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
    {
        /*
        *   Create a program in OpenGL is like generate a buffer,
        *   it simply gives back an ID for it, but it does not init
        *   anything.
        */
        unsigned int program_id = GLCall(glCreateProgram());

        /*
        *   Compile the shaders
        */
        unsigned int vs = m_CompileShader(GL_VERTEX_SHADER, vertex_shader);
        unsigned int fs = m_CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

        /*
        *   Attach the shaders, meaning to specify what is going to be linked together
        */
        GLCallVoid(glAttachShader(program_id, vs));
        GLCallVoid(glAttachShader(program_id, fs));

        /*
        * Actually link the program
        */
        GLCallVoid(glLinkProgram(program_id));
        GLCallVoid(glValidateProgram(program_id));

        int validation_sucess = 0;
        GLCallVoid(glGetProgramiv(program_id, GL_VALIDATE_STATUS, &validation_sucess));

        return program_id;
    }

    unsigned int OpenGLShader::m_CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int shader_id = GLCall(glCreateShader(type));
        const char* src = source.c_str();
        GLCallVoid(glShaderSource(shader_id, 1, &src, nullptr));
        GLCallVoid(glCompileShader(shader_id));

        int result;
        GLCallVoid(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));
        if (result == GL_FALSE)
        {
            int log_length;
            GLCallVoid(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length));
            char* error_message = (char*)alloca(log_length * sizeof(char)); // using alloca function to dynamically allocate this string on the stack (might stack overflow?)
            GLCallVoid(glGetShaderInfoLog(shader_id, log_length, &log_length, error_message));

            if (type == GL_VERTEX_SHADER)
            {
                CY_CORE_ERROR("Failed to compile vertex shader");
            }
            else
            {
                CY_CORE_ERROR("Failed to compile fragment shader");
            }
            CY_CORE_ERROR("Error message: {0}", error_message);
            std::cout << "Error message: " << error_message << '\n';
            GLCallVoid(glDeleteShader(shader_id));
            return 0;
        }

        return shader_id;
    }

    int OpenGLShader::m_GetUniformLocation(const std::string& name)
    {
        auto search_retval = m_uniformLocationMap.find(name);
        if (search_retval != m_uniformLocationMap.end() &&
            search_retval->second != -1)
        {
            return m_uniformLocationMap[name];
        }

        int location = GLCall(glGetUniformLocation(m_rendererId, name.c_str()));
        if (location == -1)
        {
            CY_CORE_INFO("Something is wrong with the uniform {0}. Maybe it was not initialized? ", name);
        }

        m_uniformLocationMap[name] = location;
        return location;
    }

    void OpenGLShader::SetUniform1i(const std::string& name, int value)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform1i(location, value));
    }

    void OpenGLShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform4f(location, v0, v1, v2, v3));
    }

    void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
}//CatolYeah