#pragma once
#include <string>
#include <source_location>
#include <format>

namespace NuEngine::Core
{
    struct AppError
    {
        std::string message;
        std::string file;
        uint32_t line;

        AppError(std::string msg, std::source_location loc = std::source_location::current())
            : message(std::move(msg))
            , file(loc.file_name())
            , line(loc.line())
        {
            auto lastSlash = file.find_last_of("/\\");
            if (lastSlash != std::string::npos) file = file.substr(lastSlash + 1);
        }

        [[nodiscard]] std::string ToString() const
        {
            return std::format("{} [Origin: {}:{}]", message, file, line);
        }
    };
}