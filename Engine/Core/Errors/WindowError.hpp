// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <string>

#include <Core/Types/Types.hpp>

namespace NuEngine::Core
{
    /*
    * @brief Error codes for window operations.
    */
    enum class WindowError
    {
        Success,                // Operation completed successfully.
        InvalidConfig,          // Invalid configuration parameters.
        PlatformFailure,        // Platform-specific API failure.
        OutOfMemory,            // Memory allocation failed.
        AlreadyInitialized,     // Window already initialized.
        GLContextFailed,        // OpenGL context creation failed.
        VulkanSurfaceFailed     // Vulkan surface creation failed.
    };

    [[nodiscard]] NU_FORCEINLINE std::string ToString(WindowError error) {
        switch (error)
        {
            case WindowError::Success: return "Success";
            case WindowError::InvalidConfig: return "Invalid configuration parameters";
            case WindowError::OutOfMemory: return "Out of memory";
            case WindowError::PlatformFailure: return "Platform failure";
            case WindowError::AlreadyInitialized: return "Already initialized";
            case WindowError::GLContextFailed: return "GL context failed";
            case WindowError::VulkanSurfaceFailed: return "Vulkan surface failed";
        default: 
            return "Unknown error";
        }
    }
}