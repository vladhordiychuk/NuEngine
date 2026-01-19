// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IRenderDevice.hpp>
#include <Graphics/Abstractions/IShader.hpp>
#include <Graphics/Abstractions/IVertexArray.hpp>
#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>

namespace NuEngine::Renderer
{
	/*
	* 
	*/
	class ForwardPipeline
	{
	public:
		/*
		* 
		*/
		ForwardPipeline(Graphics::IRenderDevice* device);

		void Initialize();

		/*
		* 
		*/
		Core::Result<void, Graphics::GraphicsError> Render() noexcept;

		void SetViewport(int x, int y, int width, int height) noexcept;

	private:
		Graphics::IRenderDevice* m_Device;

		std::shared_ptr<Graphics::IShader> m_Shader;
		std::shared_ptr<Graphics::IVertexArray> m_QuadVAO;
	};
}