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

namespace NuEngine::Graphics
{
	/**
	 * @brief Error codes for graphics operations.
	 */
	enum class GraphicsErrorCode
	{
		Success,
		PlatformFailure,
		DeviceLost,
		DriverFailure,
		CompilationFailed,
		ResourceLoadFailed,
		ResourceCreationFailed,
		OutOfMemory,
		InvalidParameter,
		UnsupportedFeature,
		InvalidWindow,
		ContextCreationFailed,
		FunctionLoadFailed,
		InvalidContext,
		SwapBuffersFailed
	};

	[[nodiscard]] constexpr std::string_view ToErrorString(GraphicsErrorCode code) noexcept
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
			default: 
				return "Unknown graphics error";
		}
	}

	constexpr size_t MAX_ERROR_TRACE_DEPTH = 8;

	/**
	 * @brief Stack trace container without dynamic allocation.
	 */
	struct ErrorTrace
	{
		std::array<Core::ErrorContext, MAX_ERROR_TRACE_DEPTH> frames;
		uint8_t count = 0;

		constexpr void Push(const Core::ErrorContext& ctx) noexcept
		{
			if (count < MAX_ERROR_TRACE_DEPTH)
			{
				frames[count++] = ctx;
			}
		}

		[[nodiscard]] constexpr bool Empty() const noexcept { return count == 0; }
	};

	/**
	 * @brief Specialized Graphics Error.
	 * Designed to be cheap to construct and copy (on stack).
	 */
	struct GraphicsError
	{
		GraphicsErrorCode code;
		Core::ErrorSeverity severity;

		std::string details;

		ErrorTrace trace;

		GraphicsError(GraphicsErrorCode c, std::string d = "",
			Core::ErrorContext ctx = {},
			Core::ErrorSeverity sev = Core::ErrorSeverity::Error) noexcept
			: code(c)
			, severity(sev)
			, details(std::move(d))
		{
			trace.Push(ctx);
		}

		bool operator==(GraphicsErrorCode c) const noexcept { return code == c; }
		bool operator!=(GraphicsErrorCode c) const noexcept { return code != c; }

		template <typename OutputIt>
		void FormatTo(OutputIt out) const
		{
			std::format_to(out, "GraphicsError: {}", ToErrorString(code));

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

	inline std::ostream& operator<<(std::ostream& os, const GraphicsError& e)
	{
		return os << e.ToString();
	}
}

template <>
struct std::formatter<NuEngine::Graphics::GraphicsError> : std::formatter<std::string> {
	auto format(const NuEngine::Graphics::GraphicsError& err, format_context& ctx) const {
		err.FormatTo(ctx.out());
		return ctx.out();
	}
};