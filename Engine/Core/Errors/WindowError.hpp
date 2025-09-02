// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

namespace NuEngine::Core
{
	/*
	* Error codes for window operations
	*/
    enum class WindowError
    {
        Success,            // Operation completed successfully
        InvalidConfig,      // Invalid configuration parameters
        PlatformFailure,    // Platform-specific API failure
        OutOfMemory,        // Memory allocation failed
        AlreadyInitialized, // Window already initialized
        GLContextFailed,    // OpenGL context creation failed
        VulkanSurfaceFailed // Vulkan surface creation failed
    };
}