// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <glad/glad.h>

namespace NuEngine::Renderer::OpenGL
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