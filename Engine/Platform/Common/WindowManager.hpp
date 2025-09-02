#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include <Platform/IWindow.hpp>

namespace NuEngine::Platform
{
    class WindowManager
    {
    public:
        static WindowManager& Get();

        WindowPtr CreateWindow(const WindowConfig& config);
        bool DestroyWindow(WindowID id);

        WindowPtr GetWindow(WindowID id);
        WindowPtr GetMainWindow();

        void ProcessAllEvents();
        void SwapAllBuffers();

        std::vector<WindowPtr> GetAllWindows();
        void SetMainWindow(WindowID id);

    private:
        std::unordered_map<WindowID, WindowPtr> m_Windows;
        WindowID m_MainWindowID = 0;
        WindowID m_NextID = 1;
    };
}