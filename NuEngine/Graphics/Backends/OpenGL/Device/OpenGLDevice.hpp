// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IRenderDevice.hpp>
#include <Graphics/Abstractions/IGraphicsContext.hpp>
#include <Core/Types/Result.hpp>

#include <memory>

namespace NuEngine::Graphics 
{ 
	class IShader; 
}

namespace NuEngine::Graphics::OpenGL
{
	typedef unsigned int GLuint;
	typedef int GLint;

	/*
	* 
	*/
	class OpenGLDevice : public IRenderDevice
	{
	public:
		OpenGLDevice(std::unique_ptr<IGraphicsContext> context);
		~OpenGLDevice() override;

		[[nodiscard]] Core::Result<std::shared_ptr<IShader>, GraphicsError> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> Clear(float r, float g, float b, float a) noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> DrawTriangle() noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> Present() noexcept override;
		void SetViewport(int x, int y, int width, int height) noexcept override;

	private:
		void CreatePipelineResources();

		std::unique_ptr<IGraphicsContext> m_Context;
		std::shared_ptr<IShader> m_Shader;

		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		GLuint m_EBO = 0;
	};
}