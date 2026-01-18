// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <array>
#include <string>
#include <string_view>
#include <format>
#include <ostream>

#include <Core/Types/Types.hpp>
#include <Core/Types/ErrorContext.hpp>

namespace NuEngine::Platform
{
	/**
	 * @brief Error codes for window operations.
	 */
	enum class WindowErrorCode
	{
		Success,                // Operation completed successfully.
		InvalidConfig,          // Invalid configuration parameters.
		PlatformFailure,        // Platform-specific API failure.
		OutOfMemory,            // Memory allocation failed.
		AlreadyInitialized,     // Window already initialized.
		GLContextFailed,        // OpenGL context creation failed.
		VulkanSurfaceFailed,    // Vulkan surface creation failed.
		CreationFailed,         // Window creation failed
		ContextLost             // Graphics context lost
	};

	[[nodiscard]] constexpr std::string_view ToErrorString(WindowErrorCode code) noexcept
	{
		switch (code)
		{
		case WindowErrorCode::Success: return "Success";
		case WindowErrorCode::InvalidConfig: return "Invalid configuration parameters";
		case WindowErrorCode::PlatformFailure: return "Platform failure";
		case WindowErrorCode::OutOfMemory: return "Out of memory";
		case WindowErrorCode::AlreadyInitialized: return "Already initialized";
		case WindowErrorCode::GLContextFailed: return "GL context failed";
		case WindowErrorCode::VulkanSurfaceFailed: return "Vulkan surface failed";
		case WindowErrorCode::CreationFailed: return "Creation failed";
		case WindowErrorCode::ContextLost: return "Context lost";
		default: return "Unknown window error";
		}
	}

	constexpr size_t MAX_WINDOW_ERROR_TRACE_DEPTH = 8;

	struct WindowErrorTrace
	{
		std::array<Core::ErrorContext, MAX_WINDOW_ERROR_TRACE_DEPTH> frames;
		uint8_t count = 0;

		constexpr void Push(const Core::ErrorContext& ctx) noexcept
		{
			if (count < MAX_WINDOW_ERROR_TRACE_DEPTH)
			{
				frames[count++] = ctx;
			}
		}

		[[nodiscard]] constexpr bool Empty() const noexcept { return count == 0; }
	};

	struct WindowError
	{
		WindowErrorCode code;
		Core::ErrorSeverity severity;
		std::string details;
		WindowErrorTrace trace;

		WindowError(WindowErrorCode c, std::string d = "",
			Core::ErrorContext ctx = {},
			Core::ErrorSeverity sev = Core::ErrorSeverity::Error) noexcept
			: code(c)
			, severity(sev)
			, details(std::move(d))
		{
			trace.Push(ctx);
		}

		bool operator==(WindowErrorCode c) const noexcept { return code == c; }
		bool operator!=(WindowErrorCode c) const noexcept { return code != c; }

		template <typename OutputIt>
		void FormatTo(OutputIt out) const
		{
			std::format_to(out, "WindowError: {}", ToErrorString(code));

			if (!details.empty())
			{
				std::format_to(out, " -> {}", details);
			}

			if (!trace.Empty())
			{
				std::format_to(out, "\nTrace:");
				for (size_t i = 0; i < trace.count; ++i)
				{
					std::format_to(out, "\n  [{}] {}:{} ({})",
						i, trace.frames[i].file, trace.frames[i].line, trace.frames[i].function);
				}
			}
		}

		[[nodiscard]] std::string ToString() const
		{
			std::string buffer;
			buffer.reserve(256 + details.size());
			FormatTo(std::back_inserter(buffer));
			return buffer;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const WindowError& e)
	{
		return os << e.ToString();
	}
}

template <>
struct std::formatter<NuEngine::Platform::WindowError> : std::formatter<std::string> {
	auto format(const NuEngine::Platform::WindowError& err, format_context& ctx) const {
		err.FormatTo(ctx.out());
		return ctx.out();
	}
};