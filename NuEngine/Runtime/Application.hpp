// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

#include <Core/Types/Types.hpp>
#include <Core/IO/FileSystem.hpp>
#include <Core/Types/Result.hpp>
#include <Runtime/Errors/EngineError.hpp>
#include <Renderer/Pipelines/Forward/ForwardPipeline.hpp>
#include <Graphics/Abstractions/Core/IRenderDevice.hpp>
#include <Platform/IWindow.hpp>

#include <memory>
#include <string>

namespace NuEngine::Runtime {

    /**
     * @brief The main class of the program that manages the lifecycle of the game engine.
     *
     * Responsible for initialization, the main loop, and termination of operation.
     */
    class Application
    {
    public:
        /**
         * @brief Application state enum
         */
        enum class AppState
        {
            Created,
            Initialized,
            Running,
            ShuttingDown,
            Terminated
        };

        /**
         * @brief Default Constructor.
         */
        Application() noexcept;

        /**
         * @brief Destructor that ensures correct termination of operation.
         */
        ~Application() noexcept;

        /**
         * @brief Copy constructor (deleted).
         */
        Application(const Application& other) = delete;

        /**
         * @brief Move constructor.
         */
        Application(Application&& other) noexcept = default;

        /**
         * @brief Copy assignment operator (deleted).
         */
        Application& operator=(const Application& other) = delete;

        /**
         * @brief Move assignment operator.
         */
        Application& operator=(Application&& other) noexcept = default;

        /**
         * @brief Starts the main program loop.
         *
         * @return Result with void (success) or EngineError.
         */
        [[nodiscard]] Core::Result<void, EngineError> Run() noexcept;

    protected:
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}

    private:
        /**
         * @brief Initializes subsystems (windows, etc.).
         */
        [[nodiscard]] Core::Result<void, EngineError> Initialize() noexcept;

        /**
         * @brief Completes the subsystem work.
         */
        [[nodiscard]] Core::Result<void, EngineError> Shutdown() noexcept;

        /**
         * @brief Executes the main program loop (event handling, rendering).
         */
        [[nodiscard]] Core::Result<void, EngineError> MainLoop() noexcept;

        /**
         * @brief Creates platform window
         */
        [[nodiscard]] Core::Result<std::unique_ptr<Platform::IWindow>, EngineError> CreateAppWindow() noexcept;

        /**
         * @brief Creates render device
         */
        [[nodiscard]] Core::Result<std::unique_ptr<Graphics::IRenderDevice>, EngineError>
            CreateRenderDevice(Platform::IWindow* window) noexcept;

        /**
         * @brief Polls system events
         */
        [[nodiscard]] Core::Result<void, EngineError> PollEvents() noexcept;

        void OnEvent(Platform::WindowEvent& event) noexcept;

        [[nodiscard]] bool OnWindowClose(Platform::WindowClosedEvent& event) noexcept;

        [[nodiscard]] bool OnWindowResize(Platform::WindowResizedEvent& event) noexcept;

        /**
         * @brief Updates application state
         */
        [[nodiscard]] Core::Result<void, EngineError> Update() noexcept;

        /**
         * @brief Renders frame
         */
        [[nodiscard]] Core::Result<void, EngineError> Render() noexcept;

        bool m_isRunning = false;
        AppState m_state = AppState::Created;
        std::unique_ptr<Platform::IWindow> m_window;
        std::unique_ptr<Renderer::ForwardPipeline> m_pipeline;
        std::unique_ptr<Graphics::IRenderDevice> m_renderDevice;
        Core::FileSystem m_fileSystem;
    };

}