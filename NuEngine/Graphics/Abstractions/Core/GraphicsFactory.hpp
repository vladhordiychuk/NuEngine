// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <Graphics/Abstractions/Core/IRenderDevice.hpp>
#include <Platform/IWindow.hpp>
#include <memory>

namespace NuEngine::Graphics
{
    /*
    * @enum GraphicsAPI
    * @brief Supported graphics APIs.
    *
    * Add a new backend here if you implement support for another API.
    */
    enum class GraphicsAPI
    {
        OpenGL,   //Use OpenGL backend 
        Vulkan,   // Use Vulkan backend (not yet implemented)
        DirectX,  // Use DirectX backend (not yet implemented)
    };

    /*
    * @class GraphicsFactory
    * @brief Factory class to create IRenderDevice instances.
    *
    * This class encapsulates the creation of a rendering device
    * for a chosen graphics API, including platform-specific context
    * initialization.
    */
    class GraphicsFactory
    {
    public:
        /*
        * @brief Creates a rendering device for the specified graphics API.
        *
        * @param api The graphics API to use (OpenGL, Vulkan, DirectX)
        * @param window Pointer to the platform window where rendering will occur
        * @return A Result containing a unique pointer to IRenderDevice on success or a GraphicsError on failure.
        *
        * @note Vulkan and DirectX are currently not implemented.
        */
        [[nodiscard]] static Core::Result<std::unique_ptr<IRenderDevice>, GraphicsError>
            CreateDevice(GraphicsAPI api, Platform::IWindow* window) noexcept;
    };
}
