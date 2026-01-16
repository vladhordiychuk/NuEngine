// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <fstream>
#include <source_location>
#include <format>
#include <mutex>
#include <memory>
#include <Core/Types/Result.hpp>
#include <Core/Errors/FileSystemError.hpp>

namespace NuEngine::Core
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

    /**
     * @brief Thread-safe logger.
     */
    class Logger
    {
    public:
        static Result<void, FileSystemError> Init(const std::string& path) noexcept;

        static void Log(LogLevel level, const std::string& message,
            const std::source_location& location = std::source_location::current()) noexcept;

        static void SetLogFile(const std::string& path) noexcept;
        static void SetMinimumLevel(LogLevel level) noexcept;
        static void EnableColors(bool enable) noexcept;
        static void Shutdown() noexcept;

    private:
        struct LoggerState
        {
            std::ofstream logFile;
            std::string logPath = "logs/nuengine.log";
            LogLevel minLevel = LogLevel::Trace;
            bool colorsEnabled = true;
            bool initialized = false;
            std::mutex mutex;
        };

        static LoggerState& GetState() noexcept;
        static void EnsureInitialized(LoggerState& state) noexcept;

        // Внутрішня версія Init без блокування (викликається, коли mutex вже заблоковано)
        static Result<void, FileSystemError> InitInternal(LoggerState& state, const std::string& path) noexcept;

        static Result<void, FileSystemError> CreateLogDirectory(const std::string& logPath) noexcept;
        static const char* GetLevelString(LogLevel level) noexcept;

#ifdef _WIN32
        static unsigned short GetWinColor(LogLevel level) noexcept;
#else
        static const char* GetAnsiColor(LogLevel level) noexcept;
#endif
    };
}

// Macros
#define LOG_TRACE(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Trace, std::format(__VA_ARGS__))
#define LOG_DEBUG(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Debug, std::format(__VA_ARGS__))
#define LOG_INFO(...)     NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Info, std::format(__VA_ARGS__))
#define LOG_WARNING(...)  NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Warning, std::format(__VA_ARGS__))
#define LOG_ERROR(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Error, std::format(__VA_ARGS__))
#define LOG_CRITICAL(...) NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Critical, std::format(__VA_ARGS__))
#define LOG_FATAL(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Critical, std::format(__VA_ARGS__))