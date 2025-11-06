#include <Core/Logging/Logger.hpp>
#include <Core/IO/FileSystem.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#endif

namespace NuEngine::Core
{
    Result<void, FileSystemError> Logger::Init(const std::string& logFilePath) noexcept
    {
        FileSystem fs("logs/");
        if (!fs.FileExists(logFilePath))
        {
            auto writeResult = fs.WriteFile(logFilePath, {});
            if (writeResult.IsError())
            {
                return Err(writeResult.UnwrapError());
            }
        }

        s_logFile.open(logFilePath, std::ios::out | std::ios::app);
        if (!s_logFile.is_open())
        {
            return Err(FileSystemError::WriteFailed);
        }
        return Ok<FileSystemError>();
    }

    void Logger::Shutdown() noexcept
    {
        if (s_logFile.is_open())
            s_logFile.close();
    }

    void Logger::SetMinimumLevel(LogLevel level) noexcept
    {
        s_minLevel = level;
    }

    void Logger::EnableColors(bool enable) noexcept
    {
        s_colorsEnabled = enable;
    }

    void Logger::Log(LogLevel level, const std::string& message, const std::source_location& location) noexcept
    {
        if (level < s_minLevel)
            return;

        if (!s_logFile.is_open() && level >= s_minLevel)
        {
            std::cerr << "Logger not initialized: " << message << std::endl;
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &timeT);
#else
        localtime_r(&timeT, &localTime);
#endif

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ) % 1000;

        std::ostringstream timeStream;
        timeStream << std::put_time(&localTime, "%H:%M:%S")
            << "." << std::setw(3) << std::setfill('0') << ms.count();

        std::string formatted = "[" + timeStream.str() + "] [" +
            std::string(GetLevelString(level)) + "] " + message +
            " \t(" + location.file_name() + ":" + std::to_string(location.line()) + ")";

#ifdef _WIN32
        if (s_colorsEnabled)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            WORD originalAttributes = consoleInfo.wAttributes;

            SetConsoleTextAttribute(hConsole, GetWinColor(level));
            std::cout << formatted << std::endl;
            SetConsoleTextAttribute(hConsole, originalAttributes);
        }
        else
        {
            std::cout << formatted << std::endl;
        }
#else
        if (s_colorsEnabled)
        {
            std::cout << GetAnsiColor(level) << formatted << RESET << std::endl;
        }
        else
        {
            std::cout << formatted << std::endl;
        }
#endif

        if (s_logFile.is_open())
            s_logFile << formatted << std::endl;
    }


    const char* Logger::GetLevelString(LogLevel level) noexcept
    {
        switch (level)
        {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO";
        case LogLevel::Warning:  return "WARNING";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default:                 return "UNKNOWN";
        }
    }

#ifdef _WIN32
    unsigned short Logger::GetWinColor(LogLevel level) noexcept
    {
        switch (level)
        {
        case LogLevel::Trace:    return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        case LogLevel::Debug:    return FOREGROUND_BLUE | FOREGROUND_GREEN;
        case LogLevel::Info:     return FOREGROUND_GREEN;
        case LogLevel::Warning:  return FOREGROUND_RED | FOREGROUND_GREEN;
        case LogLevel::Error:    return FOREGROUND_RED;
        case LogLevel::Critical: return BACKGROUND_RED | FOREGROUND_INTENSITY;
        default:                 return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
        }
    }
#else
    const char* Logger::GetAnsiColor(LogLevel level) noexcept
    {
        switch (level)
        {
        case LogLevel::Trace:    return WHITE;
        case LogLevel::Debug:    return CYAN;
        case LogLevel::Info:     return GREEN;
        case LogLevel::Warning:  return YELLOW;
        case LogLevel::Error:    return RED;
        case LogLevel::Critical: return MAGENTA;
        default:                 return RESET;
        }
    }
#endif
}