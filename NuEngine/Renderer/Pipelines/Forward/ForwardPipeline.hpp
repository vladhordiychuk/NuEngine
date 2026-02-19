// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/Core/IRenderDevice.hpp>
#include <Graphics/Abstractions/Shaders/IShader.hpp>
#include <Graphics/Abstractions/Buffers/IVertexArray.hpp>
#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>
#include <Renderer/Camera.hpp>

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
		Core::Result<void, Graphics::GraphicsError> Render(bool present = true) noexcept;

		void SetViewport(int x, int y, int width, int height) noexcept;

		void SetClearColor(const NuMath::Color& color) { m_ClearColor = color; }

		std::shared_ptr<Camera> GetCamera() const { return m_Camera; }

	private:
		Graphics::IRenderDevice* m_Device;

		std::shared_ptr<Graphics::IShader> m_Shader;
		std::shared_ptr<Graphics::IVertexArray> m_QuadVAO;
		std::shared_ptr<Graphics::ITexture> m_Texture;
		std::shared_ptr<Camera> m_Camera;
		NuMath::Color m_ClearColor = NuMath::Colors::Linear::Black;

		float m_RotationAngle = 0.0f;
		int m_Width;
		int m_Height;
	};
}