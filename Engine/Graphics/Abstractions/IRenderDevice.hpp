// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Result.hpp>
#include <Core/Types/Types.hpp>
#include <Core/Errors/GraphicsError.hpp>

namespace NuEngine::Graphics
{
	using NuFloat = Core::Types::NuFloat;

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
		virtual [[nodiscard]] Core::Result<void, Core::GraphicsError> Clear(NuFloat r, NuFloat g, NuFloat b, NuFloat a) noexcept = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, Core::GraphicsError> DrawTriangle() noexcept = 0;

		/*
		* @brief
		*/
		virtual [[nodiscard]] Core::Result<void, Core::GraphicsError> Present() noexcept = 0;
	};
}