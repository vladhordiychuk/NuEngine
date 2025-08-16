#include "../Graphics/OpenGL/Mesh/Mesh.hpp"
#include "../Graphics/OpenGL/Shaders/Shader.hpp"

#include "Application.hpp"
#include "Logging/Logger.hpp"
#include "../Platform/Window.hpp"

namespace NuEngine::Core::Application 
{
    Application::Application() 
    {
        LOG_INFO("Start Application");
    }

    Application::~Application() 
    {
        Shutdown();
    }

    void Application::Run() 
    {
        if (!Initialize()) 
        {
            LOG_ERROR("Failed to initialize Application.");
            return;
        }
        m_isRunning = true;
        MainLoop();
    }

    bool Application::Initialize() 
    {
        LOG_INFO("Application initialized.");
        return true;
    }

    void Application::Shutdown() 
    {
        LOG_INFO("Application shutting down.");
    }

    void Application::MainLoop() const
    {
        NuEngine::Core::WindowSystem::Window window(800, 600, "OpenGL Triangle");

        std::vector<float> vertices = 
        {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        std::vector<unsigned int> indices = { 0, 1, 2 };

        NuEngine::Renderer::OpenGL::Mesh triangleMesh(vertices, indices);

        std::string vertexShaderSource = R"(
            #version 450 core
            layout (location = 0) in vec3 aPos;
            void main() {
                glPosition = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }
        )";

        std::string fragmentShaderSource = R"(
            #version 450 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            }
        )";

        NuEngine::Renderer::OpenGL::Shader shader(vertexShaderSource, fragmentShaderSource);

        while (!window.ShouldClose() && m_isRunning) 
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Use();

            triangleMesh.Render();

            window.SwapBuffers();
            window.PollEvents();
        }
    }
}