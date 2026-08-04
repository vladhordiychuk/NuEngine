// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/ErrorTraits.hpp>
#include <Core/Types/BasicError.hpp>

namespace NuEngine::Runtime
{
	/**
	 * @brief Error codes for runtime operations.
	 */
	enum class RuntimeErrorCode : uint8_t
	{
		Success,                          ///> Operation completed successfully.
		InvalidApplicationState,          ///> Application is in an invalid state.
		ApplicationAlreadyInitialized,    ///> Application has already been initialized.
		ApplicationNotInitialized,        ///> Application has not been initialized.
		ApplicationAlreadyRunning,        ///> Application is already running.
		ApplicationNotRunning,            ///> Application is not running.
		InitializationFailed,             ///> Runtime initialization failed.
		ShutdownFailed,                   ///> Runtime shutdown failed.
		MainLoopFailed,                   ///> Main loop execution failed.
		UpdateFailed,                     ///> Runtime update failed.
		RenderFailed,                     ///> Runtime rendering failed.
		Unknown                           ///> Unknown runtime error.
	};
} // namespace NuEngine::Runtime

namespace NuEngine::Core
{
	using RuntimeErrorCode = NuEngine::Runtime::RuntimeErrorCode;

	/**
	 * @brief Error trait specialization for runtime errors.
	 *
	 * Provides the error category name and conversion of
	 * error codes to human-readable strings.
	 */
	template <>
	struct ErrorTraits<RuntimeErrorCode>
	{
		static constexpr std::string_view Category = "RuntimeError";

		/**
		 * @brief Converts a runtime error code to text.
		 *
		 * @param code Error code.
		 * @return Human-readable description.
		 */
		[[nodiscard]] static constexpr std::string_view ToString(RuntimeErrorCode code) noexcept
		{
			switch (code)
			{
				case RuntimeErrorCode::Success:
					return "Success";
				case RuntimeErrorCode::InvalidApplicationState:
					return "Invalid application state";
				case RuntimeErrorCode::ApplicationAlreadyInitialized:
					return "Application is already initialized";
				case RuntimeErrorCode::ApplicationNotInitialized:
					return "Application is not initialized";
				case RuntimeErrorCode::ApplicationAlreadyRunning:
					return "Application is already running";
				case RuntimeErrorCode::ApplicationNotRunning:
					return "Application is not running";
				case RuntimeErrorCode::InitializationFailed:
					return "Runtime initialization failed";
				case RuntimeErrorCode::ShutdownFailed:
					return "Runtime shutdown failed";
				case RuntimeErrorCode::MainLoopFailed:
					return "Main loop execution failed";
				case RuntimeErrorCode::UpdateFailed:
					return "Runtime update failed";
				case RuntimeErrorCode::RenderFailed:
					return "Runtime rendering failed";
				case RuntimeErrorCode::Unknown:
					return "Unknown runtime error";
			}

			return "Unknown Runtime Error";
		}
	};

	static_assert(ErrorTraitsConcept<ErrorTraits<RuntimeErrorCode>, RuntimeErrorCode>);
} // namespace NuEngine::Core

namespace NuEngine::Runtime
{
	using RuntimeError = Core::BasicError<RuntimeErrorCode>;
} // namespace NuEngine::Runtime