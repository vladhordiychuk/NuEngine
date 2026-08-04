// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <format>
#include <iterator>
#include <string>

#include <Core/Types/ErrorTraits.hpp>

namespace NuEngine::Core
{
	/**
	 * @brief Formats an error object into a human-readable string.
	 *
	 * Combines the error category, severity, message, optional path,
	 * system error information, stack trace and error code into
	 * a formatted diagnostic report.
	 *
	 * @tparam Error Error type satisfying the BasicError interface.
	 */
	template <typename Error>
	class ErrorFormatter
	{
	public:
		/**
		 * @brief Formats an error into a diagnostic string.
		 *
		 * @param error Error object to format.
		 *
		 * @return Formatted error description.
		 */
		[[nodiscard]] static std::string Format(const Error& error)
		{
			std::string out;
			out.reserve(256);

			auto msg = error.GetMessage();

			const auto displayMessage = msg.empty() ? std::string_view("(no message)") : msg;
			std::format_to(
				std::back_inserter(out),
				"[{}][{}] {}",
				Error::Traits::Category,
				ToString(error.GetSeverity()),
				displayMessage);

			auto path = error.GetPath();
			if (!path.empty())
			{
				std::format_to(std::back_inserter(out), "\n  Path: {}", path);
			}

			const auto& sysError = error.GetSystemError();
			if (sysError)
			{
				std::format_to(
					std::back_inserter(out),
					"\n  System: {} ({})",
					sysError.message(),
					sysError.value());
			}

			const auto& trace = error.GetTrace();
			if (!trace.Empty())
			{
				std::format_to(std::back_inserter(out), "\n  Trace:");

				uint8_t i = 0;
				for (const auto& f : trace)
				{
					if (f.Module)
					{
						std::format_to(
							std::back_inserter(out),
							"\n     [{}] [{}] {}:{}",
							i++, f.Module, f.File, f.Line);
					}
					else
					{
						std::format_to(
							std::back_inserter(out),
							"\n     [{}] {}:{}",
							i++, f.File, f.Line);
					}
				}
			}

			std::format_to(
				std::back_inserter(out),
				"\n  Code: {}",
				Error::Traits::ToString(error.GetCode()));

			return out;
		}
	};
} // namespace NuEngine::Core