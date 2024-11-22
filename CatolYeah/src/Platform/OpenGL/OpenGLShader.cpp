#include "cypch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "OpenGLCommon.h"

#include "CatolYeah/Core/Assert.h"

#include <glm/gtc/type_ptr.hpp>

static std::string s_ExtractName(std::string_view filepath)
{
    std::filesystem::path Path = filepath;
    return Path.stem().string();
}

namespace CatolYeah {
    OpenGLShader::OpenGLShader(std::string_view filepath)
    {
        //TODO: profile this against TheCherno solution to read as binary data
        
        // NOTE: This is the old way of parsing shader sources. The problem with it is that I could only parse a determined number of types of shaders.
        // Ex.: if my file contains 3 shaders (vertex, fragment and geometry) I could not use this function as it is.
        //ShaderSource shader_source = m_ParseShaderSource(filepath);

        /*CY_CORE_DEBUG("VERTEX SHADER:");
        CY_CORE_DEBUG("{0}", shader_source.VertexSource);
        CY_CORE_DEBUG("FRAGMENT SHADER:");
        CY_CORE_DEBUG("{0}", shader_source.FragmentSource);*/

        //m_rendererId = m_CreateShader(shader_source.VertexSource, shader_source.FragmentSource);

        // New way of parsing shader sources. Now the hash map allows us to parse as many shaders as we try to parse
        // by simply extending the ShaderType enum class.
        m_CreateShaderSourceMap(filepath);

        CY_CORE_DEBUG("VERTEX SHADER:");
        CY_CORE_DEBUG("{0}", m_shaderSourceMap[GL_VERTEX_SHADER]);
        CY_CORE_DEBUG("FRAGMENT SHADER:");
        CY_CORE_DEBUG("{0}", m_shaderSourceMap[GL_FRAGMENT_SHADER]);

        m_rendererId = m_CreateShader(m_shaderSourceMap);
        m_name = s_ExtractName(filepath);
        Bind();
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
        :   m_rendererId(0),
            m_name(name)
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

    void OpenGLShader::SetUniformInt1(const std::string& name, int value)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform1i(location, value));
    }

    void OpenGLShader::SetUniformFloat1(const std::string& name, float value)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform1f(location, value));
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, float v0, float v1, float v2)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform3f(location, v0, v1, v2));
    }

    void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& vector)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform3f(location, vector.x, vector.y, vector.z));
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, float v0, float v1, float v2, float v3)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform4f(location, v0, v1, v2, v3));
    }

    void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& vector)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniform4f(location, vector.x, vector.y, vector.z, vector.w));
    }

    void OpenGLShader::SetUniformMatFloat4(const std::string& name, const glm::mat4& matrix)
    {
        int location = m_GetUniformLocation(name);
        GLCallVoid(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
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
            if (line.find("#type") != std::string::npos)
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

    void OpenGLShader::m_CreateShaderSourceMap(std::string_view filepath)
    {
        std::fstream shader_file_stream(filepath.data());

        enum class ShaderType : int
        {
            NONE = -1, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER,
        };

        std::stringstream ss[2];
        ShaderType shader_type = ShaderType::NONE;

        std::string line;
        while (getline(shader_file_stream, line))
        {
            if (line.find("#type") != std::string::npos)
            {
                // If we turn the following behavior into a function we can write unit tests 
                // to verify if the shader type is being correctly set
                if (line.find("vertex") != std::string::npos)
                {
                    shader_type = ShaderType::VERTEX;
                    m_shaderSourceMap[GL_VERTEX_SHADER] = std::string("");
                }
                else if (line.find("fragment") != std::string::npos || line.find("pixel") != std::string::npos)
                {
                    shader_type = ShaderType::FRAGMENT;
                    m_shaderSourceMap[GL_FRAGMENT_SHADER] = std::string("");
                }
            }
            else if (shader_type != ShaderType::NONE)
            {
                m_shaderSourceMap[(GLenum)shader_type] += line + '\n';
            }
        }
    }

    unsigned int OpenGLShader::m_CreateShader(const std::unordered_map<GLenum, std::string> &shaderSources)
    {
        /*
        *   Create a program in OpenGL is like generate a buffer,
        *   it simply gives back an ID for it, but it does not init
        *   anything.
        */
        unsigned int program_id = GLCall(glCreateProgram());

        for (auto& kv : shaderSources)
        {
            /*
             *   Compile the shaders
            */
            unsigned int shader = m_CompileShader(kv.first, kv.second);

            /*
             *   Attach the shaders, meaning to specify what is going to be linked together
            */
            GLCallVoid(glAttachShader(program_id, shader));
        }
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

            CY_CORE_ERROR("Failed to compile shader");
            CY_CORE_ERROR("Error message: {0}", error_message);
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
}//CatolYeah