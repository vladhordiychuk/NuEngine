// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Core/Errors/GraphicsError.hpp>

namespace NuEngine::Graphics::OpenGL
{
	class OpenGLLoader
	{
	public:
		static NuEngine::Core::Result<void, NuEngine::Core::GraphicsError> LoadFunctions() noexcept;
	};
}