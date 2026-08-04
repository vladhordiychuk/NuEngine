// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>
#include <array>
#include <string_view>
#include <source_location>

#include <Core/Types/StaticString.hpp>
#include <NuEngine/Core/API.hpp>
#include <Core/Types/ErrorContext.hpp>

namespace NuEngine::Core
{
	struct CrashInfo
	{
		StaticString<256> Reason;
		StaticString<256> Function;
		StaticString<256> File;

		uint32_t Line = 0;
		uint32_t ThreadID = 0;
		int64_t Timestamp = 0;

		static constexpr uint8_t MaxTraceDepth = 16;
		std::array<ErrorContext, MaxTraceDepth> TraceFrames{};
		uint8_t TraceCount = 0;
	};
	class NU_API FatalReporter
	{
	public:
		static void Install() noexcept;

		[[noreturn]] static void Crash(std::string_view reason, const std::source_location& loc = std::source_location::current()) noexcept;

		[[noreturn]] static void ReportAndDie(const CrashInfo& info) noexcept;
	private:
		static void WriteCrashLog(const CrashInfo& info) noexcept;
		static void WriteMiniDump() noexcept;
	};
} // namespace NuEngine::Core