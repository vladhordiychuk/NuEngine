// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/ErrorTraits.hpp>
#include <Core/Types/BasicError.hpp>

namespace NuEngine::Graphics
{
	/**
	 * @brief Error codes for graphics operations.
	 */
	enum class GraphicsErrorCode : uint8_t
	{
		Success,                  ///> Operation completed successfully.
		PlatformFailure,          ///> Platform-specific API returned an error.
		DeviceLost,               ///> Graphics device was lost.
		DriverFailure,            ///> Graphics driver reported a failure.
		CompilationFailed,        ///> Shader compilation failed.
		ResourceLoadFailed,       ///> Failed to load a graphics resource.
		ResourceCreationFailed,   ///> Failed to create a graphics resource.
		OutOfMemory,              ///> Memory allocation failed.
		InvalidParameter,         ///> Invalid parameter was provided.
		UnsupportedFeature,       ///> Requested feature is not supported.
		InvalidWindow,            ///> Window handle is invalid.
		ContextCreationFailed,    ///> Failed to create the graphics context.
		FunctionLoadFailed,       ///> Failed to load graphics API functions.
		InvalidContext,           ///> Graphics context is invalid.
		SwapBuffersFailed         ///> Buffer swap operation failed.
	};
} // namespace NuEngine::Graphics


namespace NuEngine::Core
{
	using GraphicsErrorCode = NuEngine::Graphics::GraphicsErrorCode;

	/**
	 * @brief Error trait specialization for graphics errors.
	 *
	 * Provides the error category name and conversion of
	 * error codes to human-readable strings.
	 */
	template <>
	struct ErrorTraits<GraphicsErrorCode>
	{
		static constexpr std::string_view Category = "GraphicsError";

		/**
		 * @brief Converts a graphics error code to text.
		 *
		 * @param code Error code.
		 * @return Human-readable description.
		 */
		[[nodiscard]] static constexpr std::string_view ToString(GraphicsErrorCode code) noexcept
		{
			switch (code)
			{
			case GraphicsErrorCode::Success:
				return "Success";
			case GraphicsErrorCode::PlatformFailure:
				return "Platform failure";
			case GraphicsErrorCode::DeviceLost:
				return "Device lost";
			case GraphicsErrorCode::DriverFailure:
				return "Driver failure";
			case GraphicsErrorCode::CompilationFailed:
				return "Compilation failed";
			case GraphicsErrorCode::ResourceLoadFailed:
				return "Resource load failed";
			case GraphicsErrorCode::ResourceCreationFailed:
				return "Resource creation failed";
			case GraphicsErrorCode::OutOfMemory:
				return "Out of memory";
			case GraphicsErrorCode::InvalidParameter:
				return "Invalid parameter";
			case GraphicsErrorCode::UnsupportedFeature:
				return "Unsupported feature";
			case GraphicsErrorCode::InvalidWindow:
				return "Invalid window";
			case GraphicsErrorCode::ContextCreationFailed:
				return "Context creation failed";
			case GraphicsErrorCode::FunctionLoadFailed:
				return "Function load failed";
			case GraphicsErrorCode::InvalidContext:
				return "Invalid context";
			case GraphicsErrorCode::SwapBuffersFailed:
				return "Swap buffers failed";
			}

			return "Unknown Graphics Error";
		}
	};

	static_assert(ErrorTraitsConcept<ErrorTraits<GraphicsErrorCode>, GraphicsErrorCode>);
} // namespace NuEngine::Core

namespace NuEngine::Graphics
{
	using GraphicsError = Core::BasicError<GraphicsErrorCode>;
} // namespace NuEngine::Graphics