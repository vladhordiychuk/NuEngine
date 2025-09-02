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
    using NuInt32 = NuEngine::Core::Types::NuInt32;
    using NuBool = NuEngine::Core::Types::NuBool;

    struct WindowEvent
    {
        virtual ~WindowEvent() = default;
        uint64_t timestamp;
    };

    struct WindowClosedEvent : WindowEvent {};
    struct WindowResizedEvent : WindowEvent { NuInt32 width, height; };
    struct WindowMovedEvent : WindowEvent { NuInt32 x, y; };
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
        NuInt32 x, y;
    };

    struct MouseMoveEvent : WindowEvent { NuInt32 x, y; };
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