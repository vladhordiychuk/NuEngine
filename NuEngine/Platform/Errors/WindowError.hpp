// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <vector>

#include <Core/Types/Types.hpp>
#include <Core/Types/ErrorContext.hpp>

namespace NuEngine::Platform
{
    /**
     * @brief Error codes for window operations.
     */
    enum class WindowErrorCode
    {
        Success,                // Operation completed successfully.
        InvalidConfig,          // Invalid configuration parameters.
        PlatformFailure,        // Platform-specific API failure.
        OutOfMemory,            // Memory allocation failed.
        AlreadyInitialized,     // Window already initialized.
        GLContextFailed,        // OpenGL context creation failed.
        VulkanSurfaceFailed,    // Vulkan surface creation failed.
        CreationFailed,         // Window creation failed
        ContextLost             // Graphics context lost
    };

    [[nodiscard]] NU_FORCEINLINE std::string_view ToErrorString(WindowErrorCode code) noexcept
    {
        switch (code)
        {
        [[likely]] case WindowErrorCode::Success:
            return "Success";
        case WindowErrorCode::InvalidConfig:
            return "Invalid configuration parameters";
        case WindowErrorCode::PlatformFailure:
            return "Platform failure";
        case WindowErrorCode::OutOfMemory:
            return "Out of memory";
        case WindowErrorCode::AlreadyInitialized:
            return "Already initialized";
        case WindowErrorCode::GLContextFailed:
            return "GL context failed";
        case WindowErrorCode::VulkanSurfaceFailed:
            return "Vulkan surface failed";
        case WindowErrorCode::CreationFailed:
            return "Creation failed";
        case WindowErrorCode::ContextLost:
            return "Context lost";
        default:
            return "Unknown window error";
        }
    }

    struct WindowError
    {
        Core::ErrorSeverity severity;
        WindowErrorCode code;
        std::string details;
        std::vector<Core::ErrorContext> context;

        // Конструктор
        WindowError(WindowErrorCode c, std::string d = "", Core::ErrorContext ctx = {}) noexcept
            : code(c), details(std::move(d))
        {
            context.push_back(ctx);
        }

        [[nodiscard]] std::string ToString() const noexcept
        {
            std::string result(ToErrorString(code));
            if (!details.empty())
            {
                result += " (" + details + ")";
            }
            return result;
        }
    };

    // Оператор для виводу в потік (виправляє логування)
    NU_FORCEINLINE std::ostream& operator<<(std::ostream& os, const WindowError& e)
    {
        return os << e.ToString();
    }

    // Глобальна функція для сумісності з макросами
    [[nodiscard]] NU_FORCEINLINE std::string ToString(const WindowError& e)
    {
        return e.ToString();
    }
}