// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#include <Core/Logging/Logger.hpp>

#include <iostream>
#include <chrono>
#include <ctime>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace
{
#ifndef _WIN32
    constexpr const char* AnsiReset = "\033[0m";
    constexpr const char* AnsiRed = "\033[31m";
    constexpr const char* AnsiGreen = "\033[32m";
    constexpr const char* AnsiYellow = "\033[33m";
    constexpr const char* AnsiMagenta = "\033[35m";
    constexpr const char* AnsiCyan = "\033[36m";
    constexpr const char* AnsiWhite = "\033[37m";
    constexpr const char* AnsiBoldRed = "\033[1;31m";
#endif
}

namespace NuEngine::Core
{
    Logger::LoggerState& Logger::GetState() noexcept
    {
        static LoggerState state;
        return state;
    }

    Result<void, FileSystemError> Logger::Init(const std::string& path) noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        state.shuttingDown = false;
        return InitInternal(state, path);
    }

    Result<void, FileSystemError> Logger::InitInternal(LoggerState& state, const std::string& path) noexcept
    {
        if (state.initialized)
        {
            return Ok();
        }

        state.logPath = path;

        auto dirRes = CreateLogDirectory(state.logPath);
        if (dirRes.IsError()) 
        {
            return dirRes;
        }

        state.logFile.open(state.logPath, std::ios::out | std::ios::app);
        if (!state.logFile.is_open())
        {
            return Err(FileSystemError{
                FileSystemErrorCode::WriteFailed,
                "Failed to open log file",
                state.logPath
                });
        }

        state.initialized = true;
        return Ok();
    }

    void Logger::EnsureInitialized(LoggerState& state) noexcept
    {
        if (state.initialized) 
        {
            return;
        }

        auto result = InitInternal(state, state.logPath);
        if (result.IsError())
        {
            std::cerr << "[Logger Panic] Auto-init failed: "
                << result.UnwrapError() << "\n";
        }
    }

    Result<void, FileSystemError> Logger::CreateLogDirectory(const std::string& logPath) noexcept
    {
        std::filesystem::path path(logPath);
        if (!path.has_parent_path()) 
        {
            return Ok();
        }

        std::error_code ec;
        auto parent = path.parent_path();

        if (std::filesystem::exists(parent, ec)) 
        {
            return Ok();
        }

        if (!std::filesystem::create_directories(parent, ec))
        {
            return Err(FileSystemError{
                FileSystemErrorCode::DirectoryCreationFailed,
                ec.message(),
                parent.string()
                });
        }

        return Ok();
    }

    void Logger::SetLogFile(const std::string& path) noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        if (!state.initialized) state.logPath = path;
    }

    void Logger::SetMinimumLevel(LogLevel level) noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        state.minLevel = level;
    }

    void Logger::EnableColors(bool enable) noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        state.colorsEnabled = enable;
    }

    void Logger::Shutdown() noexcept
    {
        auto& state = GetState();

        std::lock_guard<std::mutex> lock(state.mutex);

        state.shuttingDown = true;

        if (state.logFile.is_open())
        {
            state.logFile.flush();
            state.logFile.close();
        }

        state.initialized = false;
        state.editorCallback = nullptr;
    }

    void Logger::Log(LogLevel level, const std::string& message, const std::source_location& location) noexcept
    {
        auto& state = GetState();

        LogCallback callbackCopy;
        bool colorsEnabled = false;

        {
            std::lock_guard<std::mutex> lock(state.mutex);

            if (state.shuttingDown)
            {
                return;
            }

            if (level < state.minLevel)
            {
                return;
            }

            EnsureInitialized(state);

            callbackCopy = state.editorCallback;
            colorsEnabled = state.colorsEnabled;
        }

        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &timeT);
#else
        localtime_r(&timeT, &localTime);
#endif

        char timeBuf[16]{};
        std::strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", &localTime);

        std::string_view file = location.file_name();
        if (auto pos = file.find_last_of("/\\"); pos != std::string_view::npos)
        {
            file = file.substr(pos + 1);
        }

        std::string formatted = std::format(
            "[{}.{:03d}] [{}] {} ({}:{})",
            timeBuf,
            static_cast<int>(ms.count()),
            GetLevelString(level),
            message,
            file,
            location.line());

        std::string timeString(timeBuf);

        if (callbackCopy)
        {
            callbackCopy(level, message, timeString);
        }

#ifdef _WIN32
        if (colorsEnabled)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            CONSOLE_SCREEN_BUFFER_INFO info;
            GetConsoleScreenBufferInfo(hConsole, &info);

            WORD original = info.wAttributes;
            SetConsoleTextAttribute(hConsole, GetWinColor(level));

            std::cout << formatted << '\n';

            SetConsoleTextAttribute(hConsole, original);
        }
        else
        {
            std::cout << formatted << '\n';
        }
#else
        if (colorsEnabled)
        {
            std::cout << GetAnsiColor(level)
                << formatted
                << AnsiReset
                << '\n';
        }
        else
        {
            std::cout << formatted << '\n';
        }
#endif

        {
            std::lock_guard<std::mutex> lock(state.mutex);

            if (state.logFile.is_open())
            {
                state.logFile << formatted << '\n';

                if (level == LogLevel::Fatal)
                {
                    state.logFile.flush();
                }
            }
        }
    }

    const char* Logger::GetLevelString(LogLevel level) noexcept
    {
        switch (level)
        {
            case LogLevel::Trace:    
                return "TRACE";
            case LogLevel::Debug:    
                return "DEBUG";
            case LogLevel::Info:     
                return "INFO ";
            case LogLevel::Warning:  
                return "WARN ";
            case LogLevel::Error:    
                return "ERROR";
            case LogLevel::Critical: 
                return "CRIT ";
            case LogLevel::Fatal:    
                return "FATAL";
            default:                 
                return "UNK  ";
        }
    }

    void Logger::SetEditorCallback(LogCallback callback) noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        state.editorCallback = std::move(callback);
    }

    void Logger::ClearEditorCallback() noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        state.editorCallback = nullptr;
    }

#ifdef _WIN32
    unsigned short Logger::GetWinColor(LogLevel level) noexcept
    {
        switch (level)
        {
            case LogLevel::Trace:    
                return 8;
            case LogLevel::Debug:    
                return FOREGROUND_BLUE | FOREGROUND_GREEN;
            case LogLevel::Info:     
                return FOREGROUND_GREEN;
            case LogLevel::Warning:  
                return FOREGROUND_RED | FOREGROUND_GREEN;
            case LogLevel::Error:    
                return FOREGROUND_RED;
            case LogLevel::Critical: 
                return FOREGROUND_RED | FOREGROUND_INTENSITY;
            case LogLevel::Fatal:    
                return BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            default:                 
                return 7;
        }
    }
#else
    const char* Logger::GetAnsiColor(LogLevel level) noexcept
    {
        switch (level)
        {
            case LogLevel::Trace:    
                return AnsiWhite;
            case LogLevel::Debug:    
                return AnsiCyan;
            case LogLevel::Info:     
                return AnsiGreen;
            case LogLevel::Warning:  
                return AnsiYellow;
            case LogLevel::Error:    
                return AnsiRed;
            case LogLevel::Critical: 
                return AnsiMagenta;
            case LogLevel::Fatal:    
                return AnsiBoldRed;
            default:                 
                return AnsiReset;
        }
    }
#endif
}