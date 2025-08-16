#include <glad/glad.h>
#include "Window.hpp"
#include <iostream>

namespace NuEngine::Core::WindowSystem
{

    Window::Window(int width, int height, const std::string& title) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            std::exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            std::exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            std::exit(EXIT_FAILURE);
        }

        SetVSync(true);
        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    }

    Window::~Window() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Window::PollEvents() const {
        glfwPollEvents();
    }

    void Window::SwapBuffers() const {
        glfwSwapBuffers(m_Window);
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SetVSync(bool enabled) {
        m_VSync = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

}
