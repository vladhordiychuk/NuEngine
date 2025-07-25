#pragma once

#include <string>
#include <glad/glad.h>

namespace Engine::Renderer::OpenGL
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Use() const;
		GLuint GetID() const { return m_ProgramID; }

	private:
		GLuint m_ProgramID;
		GLuint CompileShader(GLenum type, const std::string& source);
	};
}