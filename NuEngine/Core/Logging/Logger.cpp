#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <filesystem>

#include <Core/Logging/Logger.hpp>

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

namespace NuEngine::Core {

    Logger::LoggerState& Logger::GetState() noexcept
    {
        static LoggerState state;
        return state;
    }

    // Публічний Init - блокує м'ютекс і викликає Internal
    Result<void, FileSystemError> Logger::Init(const std::string& path) noexcept
    {
        auto& state = GetState();
        std::lock_guard<std::mutex> lock(state.mutex);
        return InitInternal(state, path);
    }

    // Приватний Init - виконує роботу без блокування (оскільки викликається під замком)
    Result<void, FileSystemError> Logger::InitInternal(LoggerState& state, const std::string& path) noexcept
    {
        if (state.initialized) {
            return Ok();
        }

        state.logPath = path;

        auto dirRes = CreateLogDirectory(state.logPath);
        if (dirRes.IsError()) return dirRes;

        state.logFile.open(state.logPath, std::ios::out | std::ios::app);
        if (!state.logFile.is_open()) {
            return Err(FileSystemError(
                FileSystemErrorCode::WriteFailed,
                state.logPath,
                "Failed to open log file"
            ));
        }

        state.initialized = true;
        return Ok();
    }

    void Logger::EnsureInitialized(LoggerState& state) noexcept
    {
        // Цей метод викликається тільки всередині функцій, які ВЖЕ тримають mutex
        if (state.initialized) return;

        // Спробуємо ініціалізувати
        auto result = InitInternal(state, state.logPath);
        if (result.IsError()) {
            std::cerr << "[Logger Panic] Auto-init failed: " << result.UnwrapError().ToString() << std::endl;
        }
    }

    Result<void, FileSystemError> Logger::CreateLogDirectory(const std::string& logPath) noexcept
    {
        std::filesystem::path path(logPath);
        if (!path.has_parent_path()) return Ok();

        std::error_code ec;
        auto parent = path.parent_path();

        if (std::filesystem::exists(parent, ec)) return Ok();

        if (!std::filesystem::create_directories(parent, ec)) {
            return Err(FileSystemError(FileSystemErrorCode::DirectoryCreationFailed, parent.string(), ec.message()));
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

        if (state.logFile.is_open()) {
            state.logFile.flush();
            state.logFile.close();
        }
        state.initialized = false;
    }

    void Logger::Log(LogLevel level, const std::string& message, const std::source_location& location) noexcept
    {
        auto& state = GetState();

        // КРИТИЧНО: Блокуємо mutex один раз на весь виклик
        std::lock_guard<std::mutex> lock(state.mutex);

        if (level < state.minLevel) return;

        EnsureInitialized(state); // Безпечно, бо ми вже під замком

        // 1. Форматування часу
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &timeT);
#else
        localtime_r(&timeT, &localTime);
#endif
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream timeStream;
        timeStream << std::put_time(&localTime, "%H:%M:%S") << "." << std::setw(3) << std::setfill('0') << ms.count();

        // 2. Отримання імені файлу
        std::string_view file = location.file_name();
        if (auto lastSlash = file.find_last_of("/\\"); lastSlash != std::string_view::npos)
            file = file.substr(lastSlash + 1);

        // 3. Форматування повідомлення
        std::string formatted = std::format("[{}] [{}] {} ({}:{})",
            timeStream.str(), GetLevelString(level), message, file, location.line());

        // 4. Вивід у консоль
#ifdef _WIN32
        if (state.colorsEnabled) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            WORD originalAttrs = consoleInfo.wAttributes;
            SetConsoleTextAttribute(hConsole, GetWinColor(level));
            std::cout << formatted << std::endl;
            SetConsoleTextAttribute(hConsole, originalAttrs);
        }
        else {
            std::cout << formatted << std::endl;
        }
#else
        if (state.colorsEnabled)
            std::cout << GetAnsiColor(level) << formatted << RESET << std::endl;
        else
            std::cout << formatted << std::endl;
#endif

        // 5. Вивід у файл
        if (state.logFile.is_open()) {
            state.logFile << formatted << std::endl;
        }
    }

    const char* Logger::GetLevelString(LogLevel level) noexcept
    {
        switch (level) {
        case LogLevel::Trace:    return "TRACE";
        case LogLevel::Debug:    return "DEBUG";
        case LogLevel::Info:     return "INFO ";
        case LogLevel::Warning:  return "WARN ";
        case LogLevel::Error:    return "ERROR";
        case LogLevel::Critical: return "FATAL";
        default: return "UNK  ";
        }
    }

#ifdef _WIN32
    unsigned short Logger::GetWinColor(LogLevel level) noexcept {
        switch (level) {
        case LogLevel::Trace:    return 8; // Gray
        case LogLevel::Debug:    return FOREGROUND_BLUE | FOREGROUND_GREEN; // Cyan
        case LogLevel::Info:     return FOREGROUND_GREEN;
        case LogLevel::Warning:  return FOREGROUND_RED | FOREGROUND_GREEN; // Yellow
        case LogLevel::Error:    return FOREGROUND_RED;
        case LogLevel::Critical:
            return BACKGROUND_RED | (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        default: return 7;
        }
    }
#else
    const char* Logger::GetAnsiColor(LogLevel level) noexcept {
        switch (level) {
        case LogLevel::Trace:    return WHITE;
        case LogLevel::Debug:    return CYAN;
        case LogLevel::Info:     return GREEN;
        case LogLevel::Warning:  return YELLOW;
        case LogLevel::Error:    return RED;
        case LogLevel::Critical: return MAGENTA;
        default: return RESET;
        }
    }
#endif
}