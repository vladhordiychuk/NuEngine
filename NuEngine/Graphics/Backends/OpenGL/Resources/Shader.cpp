#include <Graphics/Backends/OpenGL/Resources/Shader.hpp>
#include <Core/Logging/Logger.hpp>

#include <vector>

namespace NuEngine::Graphics::OpenGL
{
	Core::Result<void, GraphicsError> Shader::Initialize(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		auto vsRes = CompileShader(GL_VERTEX_SHADER, vertexSrc);
		if (vsRes.IsError())
		{
			return Core::Err(vsRes.UnwrapError());
		}
		GLuint vs = vsRes.Unwrap();

		auto fsRes = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
		if (fsRes.IsError())
		{
			glDeleteShader(vs);
			return Core::Err(fsRes.UnwrapError());
		}
		GLuint fs = fsRes.Unwrap();

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vs);
		glAttachShader(m_RendererID, fs);
		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::string errorMsg = "Unknown Link Error";
			if (maxLength > 0)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);
				errorMsg = std::string(infoLog.begin(), infoLog.end());
			}

			glDeleteProgram(m_RendererID);
			glDeleteShader(vs);
			glDeleteShader(fs);
			m_RendererID = 0;

			return Core::Err(GraphicsError(GraphicsErrorCode::CompilationFailed, errorMsg));
		}

		glDetachShader(m_RendererID, vs);
		glDetachShader(m_RendererID, fs);
		glDeleteShader(vs);
		glDeleteShader(fs);

		return Core::Ok();
	}

	Shader::~Shader()
	{
		if (m_RendererID)
		{
			glDeleteProgram(m_RendererID);
		}
	}

	Core::Result<GLuint, GraphicsError> Shader::CompileShader(GLenum type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);
		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::string errorMsg = "Unknown Compile Error";
			if (maxLength > 0)
			{
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				errorMsg = std::string(infoLog.begin(), infoLog.end());
			}

			glDeleteShader(shader);

			return Core::Err(GraphicsError(GraphicsErrorCode::CompilationFailed, errorMsg));
		}

		return Core::Ok(shader);
	}

	void Shader::Bind()
	{
		glUseProgram(m_RendererID);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	GLint Shader::GetUniformLocation(const std::string& name)
	{
		auto [it, inserted] = m_UniformLocationCache.try_emplace(name, -1);

		if (inserted)
		{
			it->second = glGetUniformLocation(m_RendererID, name.c_str());
			if (it->second == -1)
			{
				LOG_WARNING("[OpenGL] Uniform '{}' doesn't exist!", name);
			}
		}

		return it->second;
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetVec3(const std::string& name, const NuMath::Vector3& vec3)
	{
		glUniform3fv(GetUniformLocation(name), 1, vec3.Data());
	}

	void Shader::SetVec4(const std::string& name, const NuMath::Vector4& vec4)
	{
		glUniform4fv(GetUniformLocation(name), 1, vec4.Data());
	}

	void Shader::SetMat4x4(const std::string& name, const NuMath::Matrix4x4& mat4x4)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, mat4x4.Data());
	}
}