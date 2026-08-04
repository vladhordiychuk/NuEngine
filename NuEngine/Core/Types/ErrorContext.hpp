// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>
#include <source_location>

namespace NuEngine::Core
{
	/**
	 * @brief Describes the location where an error was created or propagated.
	 *
	 * Stores source file, function name, line number and an optional
	 * module name. Used to build an error stack trace.
	 */
	struct ErrorContext
	{
		const char* File = nullptr;
		const char* Function = nullptr;
		uint32_t Line = 0;
		const char* Module = nullptr;

		/**
		 * @brief Captures the current source location.
		 *
		 * @param module Optional subsystem or module name.
		 * @param loc Source location. Defaults to the current call site.
		 */
		constexpr ErrorContext(const char* module = nullptr, const std::source_location& loc = std::source_location::current()) noexcept
			: File(loc.file_name())
			, Function(loc.function_name())
			, Line(loc.line())
			, Module(module)
		{
		}
	};
} // namespace NuEngine::Core