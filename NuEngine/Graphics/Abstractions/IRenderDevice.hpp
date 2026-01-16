// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Graphics/Errors/GraphicsError.hpp>

#include <NuMath/NuMath.hpp>

namespace NuEngine::Graphics
{
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

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, GraphicsError> DrawTriangle() noexcept = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, GraphicsError> Present() noexcept = 0;
	};
}