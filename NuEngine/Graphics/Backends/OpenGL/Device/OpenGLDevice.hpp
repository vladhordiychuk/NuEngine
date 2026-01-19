// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IRenderDevice.hpp>
#include <Graphics/Abstractions/IGraphicsContext.hpp>
#include <Graphics/Abstractions/IVertexArray.hpp>
#include <Graphics/Abstractions/IShader.hpp>
#include <Core/Types/Result.hpp>

#include <Graphics/Abstractions/IVertexArray.hpp>
#include <Graphics/Abstractions/IVertexBuffer.hpp>
#include <Graphics/Abstractions/IIndexBuffer.hpp>
#include <Graphics/Abstractions/IShader.hpp>

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
		~OpenGLDevice() override = default;

		[[nodiscard]] Core::Result<std::shared_ptr<IShader>, GraphicsError> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> Clear(float r, float g, float b, float a) noexcept override;
		[[nodiscard]] std::shared_ptr<IVertexArray> CreateVertexArray();
		[[nodiscard]] std::shared_ptr<IVertexBuffer> CreateVertexBuffer(float* vertices, unsigned int size);
		[[nodiscard]] std::shared_ptr<IIndexBuffer> CreateIndexBuffer(unsigned int* indices, unsigned int count);
		[[nodiscard]] Core::Result<void, GraphicsError> DrawIndices(const std::shared_ptr<IVertexArray>& vertexArray) noexcept override;
		[[nodiscard]] Core::Result<void, GraphicsError> Present() noexcept override;
		void SetViewport(int x, int y, int width, int height) noexcept override;

	private:
		std::unique_ptr<IGraphicsContext> m_Context;
		std::shared_ptr<IShader> m_Shader;
	};
}