#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Platform/IWindow.hpp>
#include <Graphics/Abstractions/GraphicsFactory.hpp>

namespace NuEngine::Core
{
    Application::Application() noexcept
        : m_fileSystem("res/")
        , m_window(nullptr)
        , m_pipeline(nullptr)
        , m_isRunning(false)
    {
    }

    Application::~Application() noexcept
    {
        if (m_isRunning)
        {
            auto result = Shutdown();
            if (result.IsError())
            {
                LOG_ERROR("Shutdown failed: {}", ToString(result.UnwrapError()));
            }
        }
    }

    Result<void, WindowError> Application::Initialize() noexcept
    {
        auto windowResult = Platform::CreatePlatformWindow();
        if (windowResult.IsError())
        {
            LOG_ERROR("Failed to create window: {}", ToString(windowResult.UnwrapError()));
            return Err(windowResult.UnwrapError());
        }
        m_window = std::move(windowResult.Unwrap());

        NuEngine::Platform::WindowConfig config;
        config.SetTitle("NuEngine");
        config.SetSize(1280, 720);
        config.SetResizable(true);
        config.SetDecorated(true);

        auto initResult = m_window->Initialize(config);
        if (initResult.IsError())
            return Err(initResult.UnwrapError());

        auto showResult = m_window->Show();
        if (showResult.IsError())
            return Err(showResult.UnwrapError());

        auto deviceResult = Graphics::GraphicsFactory::CreateDevice(Graphics::GraphicsAPI::OpenGL, m_window.get());
        if (deviceResult.IsError())
        {
            LOG_ERROR("Failed to create render device: {}", ToString(deviceResult.UnwrapError()));
            return Err(WindowError::PlatformFailure);
        }

        m_renderDevice = std::move(deviceResult.Unwrap());

        m_pipeline = std::make_unique<Renderer::ForwardPipeline>(m_renderDevice.get());

        m_isRunning = true;
        LOG_INFO("Application initialized successfully");
        return Ok<WindowError>();
    }

    Result<void, WindowError> Application::Run() noexcept
    {
        auto initResult = Initialize();
        if (initResult.IsError())
        {
            LOG_ERROR("Application initialization failed: {}", ToString(initResult.UnwrapError()));
            return initResult;
        }

        while (m_isRunning && m_window && m_window->IsOpen())
        {
            m_isRunning = MainLoop();
        }

        return Shutdown();
    }

    Result<void, WindowError> Application::Shutdown() noexcept
    {
        m_isRunning = false;

        m_pipeline.reset();
        m_window.reset();

        LOG_INFO("Application shutdown successfully");
        return Ok<WindowError>();
    }

    NuBool Application::MainLoop() noexcept
    {
        if (m_window)
        {
            auto evResult = m_window->ProcessEvents();
            if (evResult.IsError())
            {
                LOG_ERROR("Window event processing failed: {}", ToString(evResult.UnwrapError()));
                return false;
            }
        }

        if (m_pipeline)
        {
            auto renderResult = m_pipeline->Render();
            if (renderResult.IsError())
            {
                LOG_ERROR("Rendering failed: {}", ToString(renderResult.UnwrapError()));
                return false;
            }
        }

        return true;
    }
}
