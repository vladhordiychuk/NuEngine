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

namespace NuEngine::Graphics
{
    /**
     * @brief Error codes for graphics operations.
     */
    enum class GraphicsErrorCode
    {
        Success,                    // No error
        PlatformFailure,            // Platform-related error
        DeviceLost,                 // GPU device lost
        DriverFailure,              // Graphics driver error
        ShaderCompilationFailed,    // Shader compilation error
        ResourceCreationFailed,     // Failed to create buffer/texture/etc.
        OutOfMemory,                // GPU ran out of memory
        InvalidParameter,           // Invalid arguments passed
        UnsupportedFeature,         // Feature not supported
        InvalidWindow,              // Invalid window handle
        ContextCreationFailed,      // Failed to create OpenGL context
        FunctionLoadFailed,         // Failed to load OpenGL functions
        InvalidContext,             // Invalid OpenGL context
        SwapBuffersFailed           // Failed to swap buffers
    };

    [[nodiscard]] NU_FORCEINLINE std::string_view ToErrorString(GraphicsErrorCode code) noexcept
    {
        switch (code)
        {
        [[likely]] case GraphicsErrorCode::Success:
            return "Success";
        case GraphicsErrorCode::PlatformFailure:
            return "Platform failure";
        case GraphicsErrorCode::DeviceLost:
            return "Device lost";
        case GraphicsErrorCode::DriverFailure:
            return "Driver failure";
        case GraphicsErrorCode::ShaderCompilationFailed:
            return "Shader compilation failed";
        case GraphicsErrorCode::ResourceCreationFailed:
            return "Resource creation failed";
        case GraphicsErrorCode::OutOfMemory:
            return "Out of memory";
        case GraphicsErrorCode::InvalidParameter:
            return "Invalid parameter";
        case GraphicsErrorCode::UnsupportedFeature:
            return "Unsupported feature";
        case GraphicsErrorCode::InvalidWindow:
            return "Invalid window";
        case GraphicsErrorCode::ContextCreationFailed:
            return "Context creation failed";
        case GraphicsErrorCode::FunctionLoadFailed:
            return "Function load failed";
        case GraphicsErrorCode::InvalidContext:
            return "Invalid context";
        case GraphicsErrorCode::SwapBuffersFailed:
            return "Swap buffers failed";
        default:
            return "Unknown graphics error";
        }
    }

    struct GraphicsError
    {
        Core::ErrorSeverity severity;
        GraphicsErrorCode code;
        std::string details;
        std::vector<Core::ErrorContext> context;

        GraphicsError(GraphicsErrorCode c, std::string d = "", Core::ErrorContext ctx = {}) noexcept
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

    NU_FORCEINLINE std::ostream& operator<<(std::ostream& os, const GraphicsError& e)
    {
        return os << e.ToString();
    }

    [[nodiscard]] NU_FORCEINLINE std::string ToString(const GraphicsError& e)
    {
        return e.ToString();
    }
}