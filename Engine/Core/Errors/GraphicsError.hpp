// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Core/Types/Types.hpp>

#include <string>

namespace NuEngine::Core
{
    /*
    * @brief Error codes for graphics.
    */
    enum class GraphicsError
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

    [[nodiscard]] NU_FORCEINLINE std::string ToString(GraphicsError error)
    {
        switch (error)
        {
            case GraphicsError::Success:                return "Success";
            case GraphicsError::PlatformFailure:        return "PlatformFailure";
            case GraphicsError::DeviceLost:             return "DeviceLost";
            case GraphicsError::DriverFailure:          return "DriverFailure";
            case GraphicsError::ShaderCompilationFailed:return "ShaderCompilationFailed";
            case GraphicsError::ResourceCreationFailed: return "ResourceCreationFailed";
            case GraphicsError::OutOfMemory:            return "OutOfMemory";
            case GraphicsError::InvalidParameter:       return "InvalidParameter";
            case GraphicsError::UnsupportedFeature:     return "UnsupportedFeature";
            case GraphicsError::InvalidWindow:          return "InvalidWindow";
            case GraphicsError::ContextCreationFailed:  return "ContextCreationFailed";
            case GraphicsError::FunctionLoadFailed:     return "FunctionLoadFailed";
            case GraphicsError::InvalidContext:         return "InvalidContext";
            case GraphicsError::SwapBuffersFailed:      return "SwapBuffersFailed";
        default:                                    
            return "UnknownError";
        }
    }
}