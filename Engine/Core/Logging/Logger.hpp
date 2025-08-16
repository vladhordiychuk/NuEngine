// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <fstream>
#include <source_location>
#include <format>

namespace NuEngine::Core::Logging
{
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class Logger
    {
    public:
        static void Init(const std::string& logFilePath);
        static void Shutdown();

        static void Log(LogLevel level, const std::string& message,
            const std::source_location& location = std::source_location::current());

        static void SetMinimumLevel(LogLevel level);
        static void EnableColors(bool enable);

    private:
        static const char* GetLevelString(LogLevel level);

#ifdef _WIN32
        static unsigned short GetWinColor(LogLevel level);
#else
        static const char* GetAnsiColor(LogLevel level);
#endif

    private:
        static inline LogLevel s_minLevel = LogLevel::Trace;
        static inline bool s_colorsEnabled = true;
        static inline std::ofstream s_logFile;
    };
}

#define LOG_TRACE(...)    NuEngine::Core::Logging::Logger::Log(NuEngine::Core::Logging::LogLevel::Trace, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_DEBUG(...)    NuEngine::Core::Logging::Logger::Log(NuEngine::Core::Logging::LogLevel::Debug, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_INFO(...)     NuEngine::Core::Logging::Logger::Log(NuEngine::Core::Logging::LogLevel::Info, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_WARNING(...)  NuEngine::Core::Logging::Logger::Log(NuEngine::Core::Logging::LogLevel::Warning, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_ERROR(...)    NuEngine::Core::Logging::Logger::Log(NuEngine::Core::Logging::LogLevel::Error, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_CRITICAL(...) NuEngine::Core::Logging::Logger::Log(NuEngine::Core::Logging::LogLevel::Critical, std::format(__VA_ARGS__), std::source_location::current())
