// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IRenderDevice.hpp>
#include <Graphics/Abstractions/IGraphicsContext.hpp>
#include <Core/Types/Types.hpp>

namespace NuEngine::Graphics::OpenGL
{
	using NuFloat = Core::Types::NuFloat;

	/*
	* 
	*/
	class OpenGLDevice : public IRenderDevice
	{
	public:
		OpenGLDevice(std::unique_ptr<IGraphicsContext> context);
		~OpenGLDevice() override = default;

		[[nodiscard]] Core::Result<void, Core::GraphicsError> Clear(NuFloat r, NuFloat g, NuFloat b, NuFloat a) noexcept override;
		[[nodiscard]] Core::Result<void, Core::GraphicsError> DrawTriangle() noexcept override;
		[[nodiscard]] Core::Result<void, Core::GraphicsError> Present() noexcept override;
	private:
		std::unique_ptr<IGraphicsContext> m_Context;
	};
}