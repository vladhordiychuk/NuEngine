#include <Runtime/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Platform/IWindow.hpp>
#include <Graphics/Abstractions/Core/GraphicsFactory.hpp>
#include <Graphics/Abstractions/Core/IRenderDevice.hpp>
#include <Renderer/Pipelines/Forward/ForwardPipeline.hpp>
#include <iostream>

namespace NuEngine::Runtime
{
    Application::Application() noexcept
        : m_fileSystem("res/")
        , m_window(nullptr)
        , m_pipeline(nullptr)
        , m_isRunning(false)
    {
        Core::Logger::Init("logs/nuengine.logs")
            .MapError([](auto&& err) {
                std::cerr << "[FATAL] Logger Init Failed: " << ToString(err) << std::endl;
                std::abort();
                return err;
            })
            .Ignore();
        LOG_INFO("NuEngine Constructed.");
    }

    Application::~Application() noexcept
    {
        Shutdown().Match(
            []() { LOG_INFO("NuEngine Shutdown Cleanly."); },
            [](const auto& err) { LOG_ERROR("Shutdown error: {}", ToString(err)); }
        );
        Core::Logger::Shutdown();
    }

    Core::Result<std::unique_ptr<Platform::IWindow>, EngineError> Application::CreateAppWindow() noexcept
    {
        NU_UNWRAP(window, Platform::CreatePlatformWindow());

        Platform::WindowConfig config;
        config.SetTitle("NuEngine");
        config.SetSize(1280, 720);
        config.SetResizable(true);
        config.SetDecorated(true);

        NU_CHECK(window->Initialize(config));

        window->SetEventCallback([this](Platform::WindowEvent& event) {
            this->OnEvent(event);
            });

        NU_CHECK(window->Show());

        return Core::Ok(std::move(window));
    }

    Core::Result<std::unique_ptr<Graphics::IRenderDevice>, EngineError> Application::CreateRenderDevice(Platform::IWindow* window) noexcept
    {
        NU_UNWRAP(device, Graphics::GraphicsFactory::CreateDevice(Graphics::GraphicsAPI::OpenGL, window));

        return Core::Ok(std::move(device));
    }

    Core::Result<void, EngineError> Application::Initialize() noexcept
    {
        if (m_state != AppState::Created)
        {
            return Core::Err(EngineError("Application cannot be initialized in current state", std::source_location::current()));
        }

        LOG_INFO("Initializing Subsystems...");

        NU_UNWRAP(window, CreateAppWindow());

        NU_UNWRAP(device, CreateRenderDevice(window.get()));

        auto pipeline = std::make_unique<Renderer::ForwardPipeline>(device.get());

        m_window = std::move(window);
        m_renderDevice = std::move(device);
        m_pipeline = std::move(pipeline);

        m_isRunning = true;
        m_state = AppState::Running;

        LOG_INFO("Application initialized successfully.");

        return Core::Ok();
    }

    Core::Result<void, EngineError> Application::Run() noexcept
    {
        NU_CHECK(Initialize());

        LOG_INFO("Entering main loop...");

        while (m_isRunning)
        {
            auto loopResult = MainLoop();

            if (loopResult.IsError())
            {
                LOG_CRITICAL("Main Loop Failure: {}", loopResult.UnwrapError().ToString());
                m_isRunning = false;
                return loopResult;
            }
        }

        return Shutdown();
    }

    Core::Result<void, EngineError> Application::Shutdown() noexcept
    {
        if (m_state == AppState::ShuttingDown || m_state == AppState::Terminated)
            return Core::Ok();

        m_state = AppState::ShuttingDown;

        m_pipeline.reset();
        m_renderDevice.reset();
        m_window.reset();

        m_isRunning = false;
        m_state = AppState::Terminated;

        LOG_INFO("Application shutdown successfully");

        return Core::Ok();
    }

    Core::Result<void, EngineError> Application::PollEvents() noexcept
    {
        if (m_window)
            NU_CHECK(m_window->ProcessEvents());

        return Core::Ok();
    }

    void Application::OnEvent(Platform::WindowEvent& event) noexcept
    {
        switch (event.GetType())
        {
            case Platform::EventType::WindowClose:
            {
                auto& closeEvent = static_cast<Platform::WindowClosedEvent&>(event);
                OnWindowClose(closeEvent);
                break;
            }
            case Platform::EventType::WindowResize:
            {
                auto& resizeEvent = static_cast<Platform::WindowResizedEvent&>(event);
                OnWindowResize(resizeEvent);
                break;
            }
        }
    }

    bool Application::OnWindowClose(Platform::WindowClosedEvent& event) noexcept
    {
        LOG_INFO("");
        m_isRunning = false;
        return true;
    }

    bool Application::OnWindowResize(Platform::WindowResizedEvent& event) noexcept
    {
        if (event.width == 0 || event.height == 0)
        {
            return false;
        }

        if (m_pipeline)
        {
            m_pipeline->SetViewport(0, 0, event.width, event.height);
        }

        return false;
    }

    Core::Result<void, EngineError> Application::Update() noexcept
    {
        // TODO: Calculate Delta Time correctly
        OnUpdate(0.016f);
        return Core::Ok();
    }

    Core::Result<void, EngineError> Application::Render() noexcept
    {
        if (m_pipeline)
        {
            NU_CHECK(m_pipeline->Render());
        }

        return Core::Ok();
    }

    Core::Result<void, EngineError> Application::MainLoop() noexcept
    {
        NU_CHECK(PollEvents());
        NU_CHECK(Update());
        NU_CHECK(Render());

        return Core::Ok();
    }
}
