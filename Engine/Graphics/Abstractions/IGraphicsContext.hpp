// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Core/Errors/GraphicsError.hpp>

namespace NuEngine::Graphics
{
	/*
	* @brief Interface for low-level graphics contexts (platform specific)
	*/
	class IGraphicsContext
	{
	public:
		/*
		* @brief
		*/
		virtual ~IGraphicsContext() = default;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, Core::GraphicsError> Initialize() noexcept = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, Core::GraphicsError> SwapBuffers() noexcept = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, Core::GraphicsError> MakeCurrent() noexcept = 0;
	};
}