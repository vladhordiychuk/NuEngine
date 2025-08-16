#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.hpp"
#include "Core/Logging/Logger.hpp"

namespace NuEngine::Renderer::OpenGL 
{
    Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) 
    {
        GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        m_ProgramID = glCreateProgram();
        glAttachShader(m_ProgramID, vertexShader);
        glAttachShader(m_ProgramID, fragmentShader);
        glLinkProgram(m_ProgramID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader() 
    {
        glDeleteProgram(m_ProgramID);
    }

    void Shader::Use() const {
        glUseProgram(m_ProgramID);
    }

    GLuint Shader::CompileShader(GLenum type, const std::string& source) 
    {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            LOG_ERROR("Shader compilation error: {}", infoLog);
        }

        return shader;
    }
}