// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/Shaders/IShader.hpp>
#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>
#include <NuMath/NuMath.hpp>

#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace NuEngine::Graphics::OpenGL
{
	class OpenGLShader : public IShader
	{
	public:
		OpenGLShader() = default;
		~OpenGLShader() override;

		[[nodiscard]] Core::Result<void, GraphicsError> Initialize(const std::string& vertexSrc, const std::string& fragmentSrc);

		void Bind() override;
		void Unbind() override;

		void SetInt(const std::string& name, int value) override;
		void SetFloat(const std::string& name, float value) override;
		void SetVec2(const std::string& name, const NuMath::Vector2& vec2) override;
		void SetVec3(const std::string& name, const NuMath::Vector3& vec3) override;
		void SetVec4(const std::string& name, const NuMath::Vector4& vec4) override;
		void SetColor(const std::string& name, const NuMath::Color& color) override;
		void SetMat4x4(const std::string& name, const NuMath::Matrix4x4& mat4x4) override;
	private:
		GLint GetUniformLocation(const std::string& name);

		[[nodiscard]] Core::Result<GLuint, GraphicsError> CompileShader(GLenum type, const std::string& source);
		GLuint m_RendererID = 0;
		std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};
}