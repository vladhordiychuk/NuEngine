// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>

namespace NuEngine::Graphics::OpenGL
{
	/*
	* 
	*/
	class OpenGLLoader
	{
	public:
		/*
		* 
		*/
		static Core::Result<void, GraphicsError> LoadFunctions() noexcept;
	};
}