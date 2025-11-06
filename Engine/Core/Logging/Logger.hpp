#pragma once

#include <string>
#include <fstream>
#include <source_location>
#include <format>
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

    /*
    * @brief
    * 
    * 
    */
    class Logger
    {
    public:
        /*
        * @brief Initializes the logger with a file path.
        * 
        * @param logFilePath Path to the log file.
        * @return Result with void (success) or FileSystemError.
        */
        [[nodiscard]] static Result<void, FileSystemError> Init(const std::string& logFilePath) noexcept;

        /*
        * @brief Shuts down the logger.
        */
        static void Shutdown() noexcept;

        /*
        * @brief Logs a message with the specified level.
        * 
        * @param level The log level.
        * @param message The message to log.
        * @param location Source location information.
        */
        static void Log(LogLevel level, const std::string& message,
            const std::source_location& location = std::source_location::current()) noexcept;

        /*
        * @brief Sets the minimum log level.
        * 
        * @param level The minimum level.
        */
        static void SetMinimumLevel(LogLevel level) noexcept;

        /*
        * @brief Enables or disables colored console output.
        * 
        * @param enable True to enable colors, false to disable.
        */
        static void EnableColors(bool enable) noexcept;

    private:
        static const char* GetLevelString(LogLevel level) noexcept;

#ifdef _WIN32
        static unsigned short GetWinColor(LogLevel level) noexcept;
#else
        static const char* GetAnsiColor(LogLevel level) noexcept;
#endif

    private:
        static inline LogLevel s_minLevel = LogLevel::Trace;
        static inline bool s_colorsEnabled = true;
        static inline std::ofstream s_logFile;
    };
}

#define LOG_TRACE(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Trace, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_DEBUG(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Debug, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_INFO(...)     NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Info, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_WARNING(...)  NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Warning, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_ERROR(...)    NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Error, std::format(__VA_ARGS__), std::source_location::current())
#define LOG_CRITICAL(...) NuEngine::Core::Logger::Log(NuEngine::Core::LogLevel::Critical, std::format(__VA_ARGS__), std::source_location::current())