// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

#include <Core/Types/Types.hpp>
#include <Core/IO/FileSystem.hpp>
#include <Core/Types/Result.hpp>
#include <Core/Errors/WindowError.hpp>

#include <Platform/IWindow.hpp>

#include <memory>

namespace NuEngine::Core
{
    using NuBool = NuEngine::Core::Types::NuBool;

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
        Application();

        /*
        * @brief Destructor that ensures correct termination of operation.
        */
        ~Application();

        /*
        * @brief Starts the main program loop.
        *
        * @return Result with void (success) or error WindowError.
        */
        [[nodiscard]] NuEngine::Core::Result<void, WindowError> Run() noexcept;

    private:
        /*
        * @brief Initializes subsystems (windows, etc.).
        *
        * @return Result with void (success) or error WindowError.
        */
        [[nodiscard]] NuEngine::Core::Result<void, WindowError> Initialize() noexcept;

        /*
        * @brief Completes the subsystem work.
        *
        * @return Result with void (success) or error WindowError.
        */
        [[nodiscard]] NuEngine::Core::Result<void, WindowError> Shutdown() noexcept;

        /*
        * @brief Executes the main program loop (event handling, rendering).
        *
        * @return true if the loop should continue, otherwise false.
        */
        [[nodiscard]] NuBool MainLoop() const noexcept;

        NuBool m_isRunning = false;     // Program work status.
        std::unique_ptr<Platform::IWindow> m_window;   // Pointer to platform-dependent window.
        FileSystem m_fileSystem;        // File system for working with resources.
    };
}