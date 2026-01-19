// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>
#include <Graphics/Abstractions/IVertexArray.hpp>
#include <Graphics/Abstractions/IVertexBuffer.hpp>
#include <Graphics/Abstractions/IIndexBuffer.hpp>
#include <Graphics/Abstractions/IShader.hpp>
#include <Graphics/Abstractions/ITexture.hpp>
#include <NuMath/NuMath.hpp>

#include <string>

namespace NuEngine::Graphics
{
	class IShader;

	/*
	* @brief
	*/
	class IRenderDevice
	{
	public:
		/*
		* @brief
		*/
		virtual ~IRenderDevice() = default;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, GraphicsError> Clear(float r, float g, float b, float a) noexcept = 0;

		[[nodiscard]] inline Core::Result<void, GraphicsError> Clear(const NuMath::Color& color) noexcept
		{
			return Clear(color.R(), color.G(), color.B(), color.A());
		}

		virtual [[nodiscard]] Core::Result<std::shared_ptr<IShader>, GraphicsError> CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) noexcept = 0;

		virtual [[nodiscard]] std::shared_ptr<IVertexArray> CreateVertexArray() = 0;
		virtual [[nodiscard]] std::shared_ptr<IVertexBuffer> CreateVertexBuffer(float* vertices, unsigned int size) = 0;
		virtual [[nodiscard]] std::shared_ptr<IIndexBuffer> CreateIndexBuffer(unsigned int* indices, unsigned int count) = 0;
		virtual [[nodiscard]] std::shared_ptr<ITexture> CreateTexture(const std::string& path) = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, GraphicsError> DrawIndices(const std::shared_ptr<IVertexArray>& vertexArray) noexcept = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, GraphicsError> Present() noexcept = 0;

		virtual void SetViewport(int x, int y, int width, int height) noexcept = 0;
	};
} // namespace Engine::Graphics