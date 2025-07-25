#pragma once

#include <string>
#include <GLFW/glfw3.h>

namespace Engine::Core::WindowSystem
{

    class Window
    {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void PollEvents() const;
        void SwapBuffers() const;
        bool ShouldClose() const;

        void SetVSync(bool enabled);
        bool IsVSync() const { return m_VSync; }

        GLFWwindow* GetNativeWindow() const { return m_Window; }

    private:
        bool m_VSync = true;
        GLFWwindow* m_Window;
    };

}
