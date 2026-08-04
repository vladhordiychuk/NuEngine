// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/ErrorTraits.hpp>
#include <Core/Types/BasicError.hpp>

namespace NuEngine::Platform
{
	/**
	 * @brief Error codes for window operations.
	 */
	enum class WindowErrorCode : uint8_t
	{
		Success,               ///> Operation completed successfully.
		InvalidConfig,         ///> Invalid window configuration parameters.
		PlatformFailure,       ///> Platform-specific API returned an error.
		OutOfMemory,           ///> Memory allocation failed.
		AlreadyInitialized,    ///> Window system has already been initialized.
		GLContextFailed,       ///> Failed to create the OpenGL context.
		VulkanSurfaceFailed,   ///> Failed to create the Vulkan surface.
		CreationFailed,        ///> Failed to create the window.
		ContextLost            ///> Graphics context was lost.
	};
} // namespace NuEngine::Platform

namespace NuEngine::Core
{
	using WindowErrorCode = NuEngine::Platform::WindowErrorCode;

	/**
	 * @brief Error trait specialization for window errors.
	 *
	 * Provides the error category name and conversion of
	 * error codes to human-readable strings.
	 */
	template <>
	struct ErrorTraits<WindowErrorCode>
	{
		static constexpr std::string_view Category = "WindowError";

		/**
		 * @brief Converts a window error code to text.
		 *
		 * @param code Error code.
		 * @return Human-readable description.
		 */
		[[nodiscard]] static constexpr std::string_view ToString(WindowErrorCode code) noexcept
		{
			switch (code)
			{
				case WindowErrorCode::Success:
					return "Success";
				case WindowErrorCode::InvalidConfig:
					return "Invalid configuration parameters";
				case WindowErrorCode::PlatformFailure:
					return "Platform failure";
				case WindowErrorCode::OutOfMemory:
					return "Out of memory";
				case WindowErrorCode::AlreadyInitialized:
					return "Already initialized";
				case WindowErrorCode::GLContextFailed:
					return "OpenGL context creation failed";
				case WindowErrorCode::VulkanSurfaceFailed:
					return "Vulkan surface creation failed";
				case WindowErrorCode::CreationFailed:
					return "Window creation failed";
				case WindowErrorCode::ContextLost:
					return "Graphics context lost";
			}

			return "Unknown Window Error";
		}
	};

	static_assert(ErrorTraitsConcept<ErrorTraits<WindowErrorCode>, WindowErrorCode>);
} // namespace NuEngine::Core

namespace NuEngine::Platform
{
	using WindowError = Core::BasicError<WindowErrorCode>;
} // namespace NuEngine::Platform