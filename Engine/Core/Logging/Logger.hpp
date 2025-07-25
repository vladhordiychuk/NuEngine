#pragma once

#include <string>
#include <fstream>
#include <source_location>
#include <format>

namespace Engine::Core::Logging
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

#define LOG_TRACE(...)    Engine::Core::Logging::Logger::Log(Engine::Core::Logging::LogLevel::Trace, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_DEBUG(...)    Engine::Core::Logging::Logger::Log(Engine::Core::Logging::LogLevel::Debug, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_INFO(...)     Engine::Core::Logging::Logger::Log(Engine::Core::Logging::LogLevel::Info, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_WARNING(...)  Engine::Core::Logging::Logger::Log(Engine::Core::Logging::LogLevel::Warning, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_ERROR(...)    Engine::Core::Logging::Logger::Log(Engine::Core::Logging::LogLevel::Error, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_CRITICAL(...) Engine::Core::Logging::Logger::Log(Engine::Core::Logging::LogLevel::Critical, std::format(__VA_ARGS__), std::source_location::current())
