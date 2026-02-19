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
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowMove,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,   
        MouseButtonReleased,
        MouseMoved,
        Scroll
    };

    struct WindowEvent
    {
        virtual ~WindowEvent() = default;
        virtual EventType GetType() const = 0;
        uint64_t timestamp;
    };

    struct WindowClosedEvent : WindowEvent
    {
        EventType GetType() const override { return EventType::WindowClose; }
    };

    struct WindowResizedEvent : WindowEvent
    {
        int width, height;
        EventType GetType() const override { return EventType::WindowResize; }
    };

    struct WindowFocusEvent : WindowEvent
    {
        bool focused;
        EventType GetType() const override { return EventType::WindowFocus; }
    };

    struct WindowMovedEvent : WindowEvent
    {
        int x, y;
        EventType GetType() const override { return EventType::WindowMove; }
    };

    struct KeyPressedEvent : WindowEvent
    {
        int key;
        int scancode;
        int mods;
        EventType GetType() const override { return EventType::KeyPressed; }
    };

    struct KeyReleasedEvent : WindowEvent
    {
        int key;
        int scancode;
        int mods;
        EventType GetType() const override { return EventType::KeyReleased; }
    };

    struct KeyEvent : WindowEvent
    {
        int key;
        int scancode;
        int action;
        int mods;

        EventType GetType() const override
        {
            return (action == 0) ? EventType::KeyReleased : EventType::KeyPressed;
        }
    };

    struct MouseButtonEvent : WindowEvent
    {
        int button;
        int action;
        int mods;
        int x, y;
        EventType GetType() const override { return EventType::MouseButtonPressed; }
    };

    struct MouseMoveEvent : WindowEvent
    {
        int x, y;
        EventType GetType() const override { return EventType::MouseMoved; }
    };

    struct ScrollEvent : WindowEvent
    {
        float xoffset, yoffset;
        EventType GetType() const override { return EventType::Scroll; }
    };

    class WindowEventSystem
    {
    public:
        using EventCallbackFn = std::function<void(WindowEvent&)>;

        void PushEvent(std::unique_ptr<WindowEvent> event);
        std::vector<std::unique_ptr<WindowEvent>> PollEvents();

        void SetEventCallback(const EventCallbackFn& callback);
        void ProcessCallbacks();

    private:
        std::vector<std::unique_ptr<WindowEvent>> m_Events;
        std::vector<std::unique_ptr<WindowEvent>> m_EventQueue;
        std::mutex m_Mutex;
        EventCallbackFn m_EventCallback;
    };
}