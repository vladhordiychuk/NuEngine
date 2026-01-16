// Copyright (c) 2025 Vladyslav Hordiychuk
// All rights reserved.
// Unauthorized copying or use of this file is strictly prohibited.

#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <functional>

#include <Core/Types/Types.hpp>

namespace NuEngine::Platform
{
    struct WindowEvent
    {
        virtual ~WindowEvent() = default;
        uint64_t timestamp;
    };

    struct WindowClosedEvent : WindowEvent {};
    struct WindowResizedEvent : WindowEvent { int width, height; };
    struct WindowMovedEvent : WindowEvent { int x, y; };
    struct WindowFocusEvent : WindowEvent { bool focused; };

    struct KeyEvent : WindowEvent {
        int key;
        int scancode;
        int action;
        int mods;
    };

    struct MouseButtonEvent : WindowEvent {
        int button;
        int action;
        int mods;
        int x, y;
    };

    struct MouseMoveEvent : WindowEvent { int x, y; };
    struct ScrollEvent : WindowEvent { double xoffset, yoffset; };

    class WindowEventSystem
    {
    public:
        void PushEvent(std::unique_ptr<WindowEvent> event);
        std::vector<std::unique_ptr<WindowEvent>> PollEvents();

        void SetEventCallback(const std::function<void(const WindowEvent&)>& callback);
        void ProcessCallbacks();

    private:
        std::vector<std::unique_ptr<WindowEvent>> m_Events;
        std::vector<std::unique_ptr<WindowEvent>> m_EventQueue;
        std::mutex m_Mutex;
        std::function<void(const WindowEvent&)> m_EventCallback;
    };
}