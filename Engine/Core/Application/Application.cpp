#include <Core/Application/Application.hpp>
#include <Core/Logging/Logger.hpp>
#include <Core/Timer/Time.hpp>
#include <Platform/IWindow.hpp>
#include <Graphics/OpenGL/OpenGLContext.hpp>

#include <glad/glad.h>

namespace NuEngine::Core
{
    Application::Application()
        : m_fileSystem("res/")
        , m_window(nullptr)
        , m_glContext(nullptr)
        , m_isRunning(false)
    {
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

    Result<void, WindowError> Application::Initialize() noexcept
    {
        auto windowResult = Platform::CreatePlatformWindow();
        if (windowResult.IsError())
        {
            LOG_ERROR("Failed to create window: {}", ToString(windowResult.UnwrapError()));
            return Err(windowResult.UnwrapError());
        }
        m_window = std::move(windowResult.Unwrap());

        std::cerr << "Initializing window..." << std::endl;
        NuEngine::Platform::WindowConfig config;
        config.SetTitle("NuEngine Window");
        config.SetSize(1280, 720);
        config.SetResizable(true);
        config.SetDecorated(true);
        auto initResult = m_window->Initialize(config);
        if (initResult.IsError()) 
        {
            return Err(initResult.UnwrapError());
        }

        auto showResult = m_window->Show();
        if (showResult.IsError())
        {
            LOG_ERROR("Failed to show window: {}", ToString(showResult.UnwrapError()));
            return Err(showResult.UnwrapError());
        }

        auto glContextResult = Graphics::OpenGL::CreatePlatformOpenGLContext(m_window.get());
        if (glContextResult.IsError())
        {
            LOG_ERROR("Failed to create OpenGL context: {}", ToString(glContextResult.UnwrapError()));
            return Err(WindowError::PlatformFailure);
        }
        m_glContext = std::move(glContextResult.Unwrap());

        auto glInitResult = m_glContext->Initialize();
        if (glInitResult.IsError())
        {
            LOG_ERROR("Failed to initialize OpenGL context: {}", ToString(glInitResult.UnwrapError()));
            return Err(WindowError::PlatformFailure);
        }

        m_isRunning = true;
        LOG_INFO("Application initialized successfully");
        return NuEngine::Core::Ok<WindowError>();
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

        m_glContext.reset();
        m_window.reset();

        LOG_INFO("Application shutdown successfully");
        return NuEngine::Core::Ok<WindowError>();
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

        auto makeCurrentResult = m_glContext->MakeCurrent();
        if (makeCurrentResult.IsError())
        {
            LOG_ERROR("Failed to make OpenGL context current: {}", ToString(makeCurrentResult.UnwrapError()));
            return false;
        }

        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto swapResult = m_glContext->SwapBuffers();
        if (swapResult.IsError())
        {
            LOG_ERROR("Failed to swap buffers: {}", ToString(swapResult.UnwrapError()));
            return false;
        }

        return true;
    }

}
