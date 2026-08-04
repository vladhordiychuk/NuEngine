// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string_view>
#include <cstdint>

namespace NuEngine::Core
{
	/**
	 * @brief Represents the severity level of an error.
	 *
	 * Used to classify errors by their impact, allowing logging,
	 * debugging, or error handling systems to react appropriately.
	 */
	enum class ErrorSeverity : uint8_t
	{
		Info,       ///> Informational message.
		Warning,    ///> Recoverable issue or unexpected condition.
		Error,      ///> Standard error that requires attention.
		Critical,   ///> Serious error affecting a subsystem.
		Fatal       ///> Unrecoverable error requiring termination.
	};

	/**
	 * @brief Converts an error severity to its textual representation.
	 *
	 * @param severity The severity value to convert.
	 * @return A human-readable string corresponding to the severity level.
	 */
	[[nodiscard]] constexpr std::string_view ToString(ErrorSeverity severity) noexcept
	{
		switch (severity)
		{
			case ErrorSeverity::Info:
				return "Info";
			case ErrorSeverity::Warning:
				return "Warning";
			case ErrorSeverity::Error:
				return "Error";
			case ErrorSeverity::Critical:
				return "Critical";
			case ErrorSeverity::Fatal:
				return "Fatal";
		}
		return "Unknown";
	}
} // namespace NuEngine::Core