// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <cstdint>
#include <source_location>
#include <string>

namespace NuEngine::Core
{
    enum class ErrorSeverity
    {
        Warning,
        Recoverable,
        Fatal
    };

    struct ErrorContext
    {
        const char* file;
        const char* function;
        uint32_t line;

        constexpr ErrorContext(
            const std::source_location& loc = std::source_location::current()) noexcept
            : file(loc.file_name())
            , function(loc.function_name())
            , line(loc.line())
        {
        }
    };
}
