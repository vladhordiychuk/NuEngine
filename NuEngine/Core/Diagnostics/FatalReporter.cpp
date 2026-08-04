#include <atomic>
#include <exception>
#include <charconv>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <string_view>
#include <cstring>

#include <Core/Diagnostics/FatalReporter.hpp>
#include <Core/Logging/Logger.hpp>

#if defined(_WIN32)
	#include <windows.h>
	#include <dbghelp.h>
	#pragma comment(lib, "dbghelp.lib")
#elif defined(__linux__) || defined(__APPLE__)
	#include <unistd.h>
	#include <fcntl.h>
	#include <execinfo.h>
	#include <pthread.h>
#endif
namespace NuEngine::Core
{
	static thread_local CrashInfo g_crashInfo;
	static std::atomic<bool> g_handled{ false };

	static uint32_t CurrentThreadId() noexcept
	{
#if defined(_WIN32)
		return static_cast<uint32_t>(GetCurrentThreadId());
#else
		return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(pthread_self()));
#endif
	}

	static int64_t CurrentTimestamp() noexcept
	{
		using namespace std::chrono;
		return duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()).count();
	}

	void FatalReporter::Install() noexcept
	{
		std::set_terminate([]() noexcept
			{
				if (g_handled.exchange(true))
				{
					std::abort();
				}

				WriteCrashLog(g_crashInfo);
				WriteMiniDump();
				std::abort();
			});
	}

	void FatalReporter::Crash(std::string_view reason, const std::source_location& loc) noexcept
	{
		CrashInfo info;
		info.Reason = reason;
		info.File = loc.file_name();
		info.Function = loc.function_name();
		info.Line = static_cast<uint32_t>(loc.line());
		info.ThreadID = CurrentThreadId();
		info.Timestamp = CurrentTimestamp();
		info.TraceFrames[0] = ErrorContext{ nullptr, loc };
		info.TraceCount = 1;
		ReportAndDie(info);
	}

	void FatalReporter::ReportAndDie(const CrashInfo& info) noexcept
	{
		g_crashInfo = info;
		std::terminate();
	}

	void FatalReporter::WriteCrashLog(const CrashInfo& info) noexcept
	{
#if defined(_WIN32)
		HANDLE h = CreateFileA("crash.log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (h == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "FATAL: %s at %s:%u (crash.log unavailable)\n",
				info.Reason.CStr(), info.File.CStr(), info.Line);
			return;
		}
		auto write = [&](std::string_view sv) noexcept
			{
				DWORD n;
				WriteFile(h, sv.data(), static_cast<DWORD>(sv.size()), &n, nullptr);
			};
#elif defined (__linux__) || defined(__APPLE__)
		int fd = open("crash.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			fprintf(stderr, "FATAL: %s at %s:%u (crash.log unavailable: %s)\n",
				info.Reason.CStr(), info.File.CStr(), info.Line, strerror(errno));
			return;
		}
		auto write = [&](std::string_view sv) noexcept
			{
				::write(fd, sv.data(), sv.size());
			};
#endif
		write("===== NuEngine Crash =====\n");
		write("Reason: "); write(info.Reason); write("\n");
		write("File: "); write(info.File); write("\n");
		write("Function: "); write(info.Function); write("\n");
		char buf[16]{};
		auto end = std::to_chars(buf, buf + sizeof(buf), info.Line).ptr;
		write("Line: "); write({ buf, end }); write("\n");
		char tbuf[16]{};
		auto tend = std::to_chars(tbuf, tbuf + sizeof(tbuf), info.ThreadID).ptr;
		write("ThreadID: "); write({ tbuf, tend }); write("\n");
		char sbuf[24]{};
		auto send = std::to_chars(sbuf, sbuf + sizeof(sbuf), info.Timestamp).ptr;
		write("Timestamp: "); write({ sbuf, send }); write(" ms\n");
		write("Trace:\n");
		for (uint8_t i = 0; i < info.TraceCount; ++i)
		{
			const auto& f = info.TraceFrames[i];
			char ibuf[4]{};
			auto iend = std::to_chars(ibuf, ibuf + sizeof(ibuf), i).ptr;
			write(" [");
			write({ ibuf, iend });
			write("] ");
			if (f.Module)
			{
				write(f.Module);
				write(" ");
			}
			write(f.File ? f.File : "?"); write(":");
			char lbuf[16]{};
			auto lend = std::to_chars(lbuf, lbuf + sizeof(lbuf), f.Line).ptr;
			write({ lbuf, lend });
			write("\n");
		}
#if defined(_WIN32)
		CloseHandle(h);
#elif defined(__linux__) || defined(__APPLE__)
		close(fd);
#endif
	}

	void FatalReporter::WriteMiniDump() noexcept
	{
#if defined(_WIN32)
		HANDLE h = CreateFileA("crash.dmp", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (h == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "FATAL: unable to write crash.dmp\n");
			return;
		}
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), h, MiniDumpWithDataSegs, nullptr, nullptr, nullptr);
		CloseHandle(h);
#elif defined(__linux__) || defined(__APPLE__)
		void* frames[32];
		int count = backtrace(frames, 32);
		int fd = open("crash_stack.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd >= 0)
		{
			backtrace_symbols_fd(frames, count, fd);
			close(fd);
		}
		else
		{
			fprintf(stderr, "FATAL: unable to write crash_stack.log: %s\n", strerror(errno));
		}
#endif
	}
} // namespace NuEngine::Core