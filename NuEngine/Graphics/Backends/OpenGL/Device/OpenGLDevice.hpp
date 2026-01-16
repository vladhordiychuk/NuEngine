// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IRenderDevice.hpp>
#include <Graphics/Abstractions/IGraphicsContext.hpp>

#include <NuMath/NuMath.hpp>

#include <memory>

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

		[[nodiscard]] Core::Result<void, GraphicsError> Clear(float r, float g, float b, float a) noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> DrawTriangle() noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> Present() noexcept override;
	private:
		void CreatePipelineResources();

		std::unique_ptr<IGraphicsContext> m_Context;

		GLuint m_ShaderProgram = 0;
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		GLuint m_EBO = 0;
		GLint m_TransformLoc = -1;
	};
}