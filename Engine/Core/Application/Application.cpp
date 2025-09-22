#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Platform/IWindow.hpp>

namespace NuEngine::Core
{
    Application::Application()
        : m_fileSystem("res/")
    {
        auto winResult = Platform::CreatePlatformWindow();
        if (winResult.IsOk())
        {
            m_window = std::move(winResult.Unwrap());
        }
        else
        {
            m_window = nullptr;
            LOG_ERROR("Failed to create window");
        }
    }


    Application::~Application()
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

    NuEngine::Core::Result<void, NuEngine::Core::WindowError> Application::Initialize() noexcept
    {
        if (!m_window)
        {
            LOG_ERROR("Window is null");
            return Err(WindowError::PlatformFailure);
        }

        Platform::WindowConfig config;

        auto initResult = m_window->Initialize(config);
        if (initResult.IsError())
        {
            LOG_ERROR("Window initialization failed: {}", ToString(initResult.UnwrapError()));
            return initResult;
        }

        auto showResult = m_window->Show();
        if (showResult.IsError())
        {
            LOG_ERROR("Show window failed: {}", ToString(showResult.UnwrapError()));
            return showResult;
        }

        m_isRunning = true;
        LOG_INFO("Application initialized successfully");
        return NuEngine::Core::Ok<NuEngine::Core::WindowError>();
    }

    NuEngine::Core::Result<void, NuEngine::Core::WindowError> Application::Run() noexcept
    {
        auto initResult = Initialize();
        if (initResult.IsError())
        {
            LOG_ERROR("Application initialization failed: {}", ToString(initResult.UnwrapError()));
            return initResult;
        }

        while (m_isRunning && m_window->IsOpen())
        {
            m_isRunning = MainLoop();
        }

        return Shutdown();
    }

    NuEngine::Core::Result<void, NuEngine::Core::WindowError> Application::Shutdown() noexcept
    {
        m_isRunning = false;
        if (m_window)
        {
            auto shutdownResult = m_window->Shutdown();
            if (shutdownResult.IsError())
            {
                LOG_ERROR("Window shutdown failed: {}", ToString(shutdownResult.UnwrapError()));
                return shutdownResult;
            }
        }
        LOG_INFO("Application shutdown successfully");
        return NuEngine::Core::Ok<NuEngine::Core::WindowError>();
    }

    NuBool Application::MainLoop() const noexcept
    {
        auto eventResult = m_window->ProcessEvents();
        if (eventResult.IsError())
        {
            LOG_ERROR("Event processing failed: {}", ToString(eventResult.UnwrapError()));
            return false;
        }

        return true;
    }
}