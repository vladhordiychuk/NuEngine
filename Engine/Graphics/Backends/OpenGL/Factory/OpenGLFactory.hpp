// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/IRenderDevice.hpp>
#include <Platform/IWindow.hpp>
#include <Core/Types/Result.hpp>
#include <Core/Errors/GraphicsError.hpp>

#include <memory>

namespace NuEngine::Graphics::OpenGL
{
	class OpenGLFactory
	{
	public:
		[[nodiscard]] static Core::Result<std::unique_ptr<IRenderDevice>, Core::GraphicsError> 
			CreateDevice(Platform::IWindow* window) noexcept;
	};
}