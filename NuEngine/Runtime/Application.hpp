// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once 

#include <Core/Types/Types.hpp>
#include <Core/IO/FileSystem.hpp>
#include <Core/Types/Result/Result.hpp>
#include <Runtime/Errors/RuntimeError.hpp>
#include <Renderer/Pipelines/Forward/ForwardPipeline.hpp>
#include <Graphics/Abstractions/Core/IRenderDevice.hpp>
#include <Platform/IWindow.hpp>
#include <NuEngine/Core/API.hpp>

#include <NuMath/NuMath.hpp>

#include <memory>
#include <string>

namespace NuEngine::Runtime
{
    struct NU_API ApplicationSpecification
    {
        std::string Name = "NuEngine App";
        bool Windowed = true;
    };

    /**
     * @brief The main class of the program that manages the lifecycle of the game engine.
     *
     * Responsible for initialization, the main loop, and termination of operation.
     */
    class NU_API Application
    {
    public:
        /**
         * @brief Application state enum
         */
        enum class AppState
        {
            Created,
            Running,
            ShuttingDown,
            Terminated
        };

        /**
         * @brief Default Constructor.
         */
        Application(const ApplicationSpecification& spec = ApplicationSpecification()) noexcept;

        /**
         * @brief Destructor that ensures correct termination of operation.
         */
        virtual ~Application() noexcept;

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
         * @return Result with void (success) or RuntimeError.
         */
        [[nodiscard]] Core::Result<void, RuntimeError> Run() noexcept;

        /**
         * @brief Renders a single frame. Intended for externally-driven loops
         *        (e.g. an editor), as opposed to Run()'s own internal loop.
         */
        void RenderFrame();

        /**
         * @brief Processes window events and advances application state by one frame.
         *        Intended for externally-driven loops (e.g. an editor).
         */
        void UpdateFrame(float deltaTime);

        /**
         * @brief
         */
        Platform::IWindow* GetWindow() const { return m_window.get(); }

        /**
         * @brief
         */
        void InitializeGraphicsForEditor();

        /**
         * @brief
         */
        void ResizeViewport(int width, int height) noexcept
        {
            if (m_pipeline)
            {
                m_pipeline->SetViewport(0, 0, width, height);
            }
        }

        Renderer::ForwardPipeline* GetPipeline() const { return m_pipeline.get(); }

        void SetClearColor(float r, float g, float b, float a) {
            if (m_pipeline) {
                m_pipeline->SetClearColor(NuMath::Color(r, g, b, a));
            }
        }

        virtual void OnInit() {}
        virtual void OnShutdown() {}

    protected:
        /**
         * @brief
         */
        virtual void OnUpdate(float deltaTime);

        /**
         * @brief
         */
        virtual void OnRender() {}

        /**
         * @brief
         */
        virtual void OnEvent(Platform::WindowEvent& event) noexcept;

    private:
        /**
         * @brief Initializes subsystems (windows, etc.).
         */
        [[nodiscard]] Core::Result<void, RuntimeError> Initialize() noexcept;

        /**
         * @brief Completes the subsystem work.
         */
        [[nodiscard]] Core::Result<void, RuntimeError> Shutdown() noexcept;

        /**
         * @brief Executes the main program loop (event handling, rendering).
         */
        [[nodiscard]] Core::Result<void, RuntimeError> MainLoop() noexcept;

        /**
         * @brief Creates platform window
         */
        [[nodiscard]] Core::Result<std::unique_ptr<Platform::IWindow>, RuntimeError> CreateAppWindow() noexcept;

        /**
         * @brief Creates render device
         */
        [[nodiscard]] Core::Result<std::unique_ptr<Graphics::IRenderDevice>, RuntimeError>
            CreateRenderDevice(Platform::IWindow* window) noexcept;

        /**
         * @brief Polls system events
         */
        [[nodiscard]] Core::Result<void, RuntimeError> PollEvents() noexcept;

        /**
         * @brief
         */
        [[nodiscard]] bool OnWindowClose(Platform::WindowClosedEvent& event) noexcept;

        /**
         * @brief
         */
        [[nodiscard]] bool OnWindowResize(Platform::WindowResizedEvent& event) noexcept;

        /**
         * @brief Updates application state
         */
        [[nodiscard]] Core::Result<void, RuntimeError> Update() noexcept;

        /**
         * @brief Renders frame
         */
        [[nodiscard]] Core::Result<void, RuntimeError> Render() noexcept;

        bool m_isRunning = false;
        AppState m_state = AppState::Created;
        ApplicationSpecification m_specification;
        std::unique_ptr<Platform::IWindow> m_window;
        std::unique_ptr<Renderer::ForwardPipeline> m_pipeline;
        std::unique_ptr<Graphics::IRenderDevice> m_renderDevice;
        Core::FileSystem m_fileSystem;
        float m_fpsTimer = 0.0f;
        uint32_t m_frameCount = 0;
        uint32_t m_lastFPS = 0;
    };

}