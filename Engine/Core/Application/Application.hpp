// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

#include <Core/Types/Types.hpp>
#include <Core/IO/FileSystem.hpp>
#include <Core/Types/Result.hpp>
#include <Core/Errors/WindowError.hpp>
#include <Renderer/Pipeline/ForwardPipeline.hpp>
#include <Graphics/Abstractions/IRenderDevice.hpp>

#include <Platform/IWindow.hpp>

#include <memory>

namespace NuEngine::Core
{
    using NuBool = Types::NuBool;
    using NuFloat = Types::NuFloat;
    using NuInt32 = Types::NuInt32;

    /*
    * @brief The main class of the program that manages the lifecycle of the game engine.
    *
    * Responsible for initialization, the main loop, and termination of operation.
    */
    class Application
    {
    public:
        /*
        * @brief Default Constructor.
        */
        Application() noexcept;

        /*
        * @brief Destructor that ensures correct termination of operation.
        */
        ~Application() noexcept;

        /*
        * @brief Copy constructor (deleted).
        *
        * Application cannot be copied to prevent multiple ownership of resources.
        */
        Application(const Application& other) = delete;

        /*
        * @brief Move constructor.
        *
        * Transfers ownership of resources from another Application instance.
        */
        Application(Application&& other) noexcept = default;

        /*
        * @brief Copy assignment operator (deleted).
        *
        * Application cannot be copied to prevent multiple ownership of resources.
        */
        Application& operator=(const Application& other) = delete;

        /*
        * @brief Move assignment operator.
        *
        * Transfers ownership of resources from another Application instance.
        */
        Application& operator=(Application&& other) noexcept = default;

        /*
        * @brief Starts the main program loop.
        *
        * @return Result with void (success) or error WindowError.
        */
        [[nodiscard]] Result<void, WindowError> Run() noexcept;

    private:
        /*
        * @brief Initializes subsystems (windows, etc.).
        *
        * @return Result with void (success) or error WindowError.
        */
        [[nodiscard]] Result<void, WindowError> Initialize() noexcept;

        /*
        * @brief Completes the subsystem work.
        *
        * @return Result with void (success) or error WindowError.
        */
        [[nodiscard]] Result<void, WindowError> Shutdown() noexcept;

        /*
        * @brief Executes the main program loop (event handling, rendering).
        *
        * @return true if the loop should continue, otherwise false.
        */
        [[nodiscard]] NuBool MainLoop() noexcept;

        NuBool m_isRunning = false;                                         // Program work status.
        std::unique_ptr<Platform::IWindow> m_window;                        // Pointer to platform-dependent window.
        std::unique_ptr<Renderer::ForwardPipeline> m_pipeline;              // Rendering pipeline
        std::unique_ptr<Graphics::IRenderDevice> m_renderDevice;            // Render device abstraction.
        FileSystem m_fileSystem;                                            // File system for working with resources.
    };
}