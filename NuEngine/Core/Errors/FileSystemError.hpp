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

namespace NuEngine::Core
{
	enum class FileSystemErrorCode
	{
		Success,
		FileNotFound,
		PermissionDenied,
		InvalidPath,
		DirectoryCreationFailed,
		ReadFailed,
		WriteFailed,
		OutOfMemory,
		InvalidBasePath,
		PlatformFailure,
	};

	[[nodiscard]] constexpr std::string_view ToErrorString(FileSystemErrorCode code) noexcept
	{
		switch (code)
		{
			case FileSystemErrorCode::Success: 
				return "Success";
			case FileSystemErrorCode::FileNotFound: 
				return "File not found";
			case FileSystemErrorCode::PermissionDenied: 
				return "Permission denied";
			case FileSystemErrorCode::InvalidPath: 
				return "Invalid path";
			case FileSystemErrorCode::DirectoryCreationFailed:
				return "Directory creation failed";
			case FileSystemErrorCode::ReadFailed: 
				return "Read failed";
			case FileSystemErrorCode::WriteFailed: 
				return "Write failed";
			case FileSystemErrorCode::OutOfMemory: 
				return "Out of memory";
			case FileSystemErrorCode::InvalidBasePath: 
				return "Invalid base path";
			case FileSystemErrorCode::PlatformFailure: 
				return "Platform-specific API error";
			default: 
				return "Unknown error";
		}
	}

	constexpr size_t MAX_FS_ERROR_TRACE_DEPTH = 8;

	struct FsErrorTrace
	{
		std::array<Core::ErrorContext, MAX_FS_ERROR_TRACE_DEPTH> frames;
		uint8_t count = 0;

		constexpr void Push(const Core::ErrorContext& ctx) noexcept
		{
			if (count < MAX_FS_ERROR_TRACE_DEPTH)
			{
				frames[count++] = ctx;
			}
		}

		[[nodiscard]] constexpr bool Empty() const noexcept { return count == 0; }
	};

	struct FileSystemError
	{
		FileSystemErrorCode code;
		ErrorSeverity severity;
		std::string path;
		std::string details;
		FsErrorTrace trace;

		FileSystemError(FileSystemErrorCode c, std::string p = "", std::string d = "",
			Core::ErrorContext ctx = {},
			ErrorSeverity sev = ErrorSeverity::Error) noexcept
			: code(c)
			, severity(sev)
			, path(std::move(p))
			, details(std::move(d))
		{
			trace.Push(ctx);
		}

		bool operator==(FileSystemErrorCode c) const noexcept { return code == c; }
		bool operator!=(FileSystemErrorCode c) const noexcept { return code != c; }

		template <typename OutputIt>
		void FormatTo(OutputIt out) const
		{
			std::format_to(out, "FileSystemError: {}", ToErrorString(code));

			if (!path.empty())
			{
				std::format_to(out, " ['{}']", path);
			}

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
			buffer.reserve(256 + path.size() + details.size());
			FormatTo(std::back_inserter(buffer));
			return buffer;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const FileSystemError& e)
	{
		return os << e.ToString();
	}
}

template <>
struct std::formatter<NuEngine::Core::FileSystemError> : std::formatter<std::string> {
	auto format(const NuEngine::Core::FileSystemError& err, format_context& ctx) const {
		err.FormatTo(ctx.out());
		return ctx.out();
	}
};