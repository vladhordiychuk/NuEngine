// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/ITexture.hpp>
#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>
#include <glad/glad.h>

namespace NuEngine::Graphics::OpenGL
{
	class OpenGLTexture : public ITexture
	{
	public:
		OpenGLTexture(const std::string& path);
		~OpenGLTexture() override;

		[[nodiscard]] Core::Result<void, GraphicsError> Initialize();

		void Bind(unsigned int slot = 0) const override;
		void Unbind() const override;

		int GetWidth() const override { return m_Width; }
		int GetHeight() const override { return m_Height; }

		const std::string& GetPath() const override { return m_Path; }
	private:
		std::string m_Path;
		GLuint m_RendererID;
		int m_Width, m_Height, m_BPP;
		GLenum m_InternalFormat, m_DataFormat;
	};
}